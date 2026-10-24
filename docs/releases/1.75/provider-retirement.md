> **1.9 documentation:** [Current update](../1.9/README.md) · [feature status](../1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Provider selection correction

The host launch configuration selected Qwen3.8, while the persisted provider registry still selected the earlier Omni Instruct profile. Requests use the persisted selection, so changing the launch configuration alone did not change the effective model. This caused a request to reach Instruct after the intended rollback.

With no active production worker, the persisted default was corrected to Qwen3.8 and the Instruct profile was retired. The Instruct inference service and its old isolated application were stopped. Its files were retained; it is not an available fallback.

The registry now rejects selection of retired profiles and excludes retired defaults on lookup. Host initialization can recover from a retired default using the configured active profile. The native regression test covers these cases, immutable profile preservation, failed-selection preservation, reopen persistence and SQL mutation rejection. The guard is deployed in production r24. The persisted default is the separately qualified Thinking profile, with Qwen3.8 retained for rollback. Instruct remains retired and its service stopped.

Thinking's main model and matching projector are now verified against the pinned digests. Two four-turn native trials passed reasoning, corrected recall, original-source history search/read and archive verification. These passes do not establish browser or media readiness. Actual selected-voice browser testing exposed an incorrect claim of missing audio access despite verified native waveform input; that failure is retained and media acceptance remains open.

The effective selected provider must be checked from the durable run binding as well as configuration during final promotion. A model name shown in configuration alone is not sufficient evidence of the provider used for a reply.
