#!/usr/bin/env python3
"""Bound background status receipt retention; never traverse other Echo data."""
import argparse,fcntl,json,os,shutil,time
from pathlib import Path

def missing_only(function,path,error):
    # macOS removes the AppleDouble sidecar with its corresponding file.
    if not isinstance(error[1],FileNotFoundError):
        raise error[1]

def candidates(base,cutoff):
    if base.is_symlink() or not base.is_dir():
        raise ValueError('Receipt parent must be an available real directory')
    for rotation in base.iterdir():
        if not rotation.name.startswith('rest-rotation-') or rotation.is_symlink() or not rotation.is_dir():
            continue
        for service in ('activity','queue_controls'):
            service_root=rotation/service
            receipts=service_root/'receipts'
            if service_root.is_symlink() or receipts.is_symlink() or not receipts.is_dir():
                continue
            for day in receipts.iterdir():
                if day.name.isdigit() and int(day.name)<cutoff and not day.is_symlink() and day.is_dir():
                    yield day

def prune(base,keep_days=7,now=None):
    if keep_days<7:
        raise ValueError('At least seven days must be retained')
    cutoff=int((time.time() if now is None else now)//86400)-keep_days
    removed=[]
    for path in candidates(base,cutoff):
        shutil.rmtree(path,onerror=missing_only)
        removed.append(str(path))
    return {'completed_at':time.time(),'cutoff_day_exclusive':cutoff,'removed':removed,'scope':['activity','queue_controls']}

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('receipt_parent',type=Path)
    parser.add_argument('state_directory',type=Path)
    args=parser.parse_args()
    args.state_directory.mkdir(parents=True,exist_ok=True,mode=0o700)
    with (args.state_directory/'retention.lock').open('a') as lock:
        try:fcntl.flock(lock,fcntl.LOCK_EX|fcntl.LOCK_NB)
        except BlockingIOError:return
        try:result=prune(args.receipt_parent)
        except Exception as error:
            result={'completed_at':time.time(),'error':str(error),'error_type':type(error).__name__}
            status=1
        else:status=0
        temporary=args.state_directory/'retention-result.tmp'
        temporary.write_text(json.dumps(result,indent=2))
        os.replace(temporary,args.state_directory/'retention-result.json')
        raise SystemExit(status)
if __name__=='__main__':main()
