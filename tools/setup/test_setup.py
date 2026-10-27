import importlib.util,json,tempfile,unittest,hashlib,platform
from pathlib import Path
from unittest.mock import patch
spec=importlib.util.spec_from_file_location('setup',Path(__file__).with_name('echo_setup.py'));setup=importlib.util.module_from_spec(spec);spec.loader.exec_module(setup)
class SetupTests(unittest.TestCase):
 def test_shortcuts_all_platforms(self):
  with tempfile.TemporaryDirectory() as t:
   for system,suffix in [('Darwin','.webloc'),('Windows','.url'),('Linux','.desktop')]:
    p=Path(setup.desktop_entry(desktop=t,system=system));self.assertTrue(p.is_file());self.assertEqual(p.suffix,suffix)
   with self.assertRaises(ValueError):setup.desktop_entry('https://other.example',desktop=t)
 def test_hardware_choice_respects_memory_and_disk(self):
  rows=[{'minimum_ram_bytes':8,'bytes':1},{'minimum_ram_bytes':16,'bytes':2}]
  self.assertEqual(setup.select_model({'memory_bytes':12,'free_disk_bytes':10**12},rows),rows[0])
  with self.assertRaises(RuntimeError):setup.select_model({'memory_bytes':32,'free_disk_bytes':0},rows)
 def test_integrity_platform_and_traversal(self):
  with tempfile.TemporaryDirectory() as t:
   p=Path(t);(p/'test').write_text('fixture');m={'platform':'macos-arm64','files':{'test':setup.digest(p/'test')},'checks':[{'executable':'test'}]};(p/'manifest.json').write_text(json.dumps(m));self.assertEqual(setup.verify_bundle(p,{'platform_key':'macos-arm64'}),m)
   with self.assertRaises(RuntimeError):setup.verify_bundle(p,{'platform_key':'linux-x86_64'})
   (p/'test').write_text('changed')
   with self.assertRaises(RuntimeError):setup.verify_bundle(p,{'platform_key':'macos-arm64'})
   for path in ['../escape','/absolute']:
    with self.assertRaises(ValueError):setup.safe_relative(p,path)
 def test_failed_acceptance_does_not_replace_existing_installation(self):
  with tempfile.TemporaryDirectory() as t:
   p=Path(t);bundle=p/'bundle';bundle.mkdir();binary=bundle/'check';binary.write_text('#!/bin/sh\nexit 1\n');binary.chmod(0o755);m={'platform':'fixture','files':{'check':setup.digest(binary)},'executables':['check'],'checks':[{'executable':'check'}]};(bundle/'manifest.json').write_text(json.dumps(m));dest=p/'installed';dest.mkdir();(dest/'history').write_text('preserved')
   with self.assertRaises(Exception):setup.install_bundle(bundle,dest,{'platform_key':'fixture'})
   self.assertEqual((dest/'history').read_text(),'preserved')
 def test_success_requires_checks(self):
  with tempfile.TemporaryDirectory() as t:
   p=Path(t);bundle=p/'bundle';bundle.mkdir();binary=bundle/'check';binary.write_text('#!/bin/sh\nexit 0\n');m={'platform':'fixture','files':{'check':setup.digest(binary)},'executables':['check'],'checks':[{'executable':'check'}]};(bundle/'manifest.json').write_text(json.dumps(m));r=setup.install_bundle(bundle,p/'installed',{'platform_key':'fixture'});self.assertEqual(r['checks'],1);self.assertEqual(r['status'],'package-verified')
if __name__=='__main__':unittest.main()
