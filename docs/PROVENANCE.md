> **1.9 documentation:** [Current update](releases/1.9/README.md) · [feature status](releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

> **Echo 1.75 update:** [Release record](releases/1.75/README.md) and [remaining work](releases/1.75/HANDOVER.md) supersede older model/voice/watch status below. Older qualification records remain historical.

# Provenance and dependencies

## Ernos toolchain

Pinned compiler base: https://github.com/MettaMazza/Ernos-Programming-Language at `4b3ebd1359886999b4f043a4663b9470ee8d474c`. The retained `engine/ernos-overrides` directory contains the import-identity integration, generated bootstrap and fixtures, with its original provenance note. Its generated C is a bootstrap artifact, not hand-authored application code. Unrequested later code-generation experiments were removed to recoverable Trash on 15 October and were never installed. This documentation does not authorize or introduce compiler changes.

ErnosDecent reference: https://github.com/MettaMazza/ErnosDecent at `4ef202c5a920f17e5d2f6bde406b62e8185b1531`. It is a reference checkout, not vendored application source. Personal identity files are excluded upstream. It must not be confused with the current Echo application's capabilities.

## Runtime dependencies

The installation uses llama.cpp b11429, mlx-lm/MLX with a local tokenizer helper, Caddy 2.11.7, SQLite, Apple/macOS native libraries and Clang. Image generation uses stable-diffusion.cpp; speech transcription uses whisper.cpp; speech synthesis uses Kokoro-82M ONNX with kokoro-onnx and the bm_fable voice; see KOKORO.md. Browser formatting uses locally served third-party assets. The forensic dependency inventory records available versions, source identities and hashes; where a version is unknown it remains unknown rather than guessed.

Model directories currently include Qwen3.5-35B-A3B MLX 4-bit and Qwen3.8-Flash-Next Q8_0 GGUF plus its projector. Model filenames and metadata are not independent proof of quality, capacity or native modality support. Model weights and their licenses remain separate acquisitions and are not included here.

No project-wide license has been invented on the owner's behalf. Public visibility alone does not grant an open-source license. Upstream components retain their original license terms; consult each source distribution before redistribution. Generated configurations are examples, not a vendored dependency lock or universal installer.

The previous private GitHub Echo repository contained Echo.zip and README.md. A local pre-replacement copy of both files was verified against Git blob IDs. The owner chose replacement of current contents while preserving Git history. The old repository's commits therefore remain ancestors of the new source commits.
