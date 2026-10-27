$ErrorActionPreference = 'Stop'
$script = Join-Path $PSScriptRoot 'echo_setup.py'
if (Get-Command py -ErrorAction SilentlyContinue) { & py -3 $script @args }
elseif (Get-Command python -ErrorAction SilentlyContinue) { & python $script @args }
else { throw 'Python 3.10 or newer is required. No Echo installation was changed.' }
exit $LASTEXITCODE
