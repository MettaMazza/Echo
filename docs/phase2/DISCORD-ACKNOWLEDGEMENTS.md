> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Discord acknowledgement deadline

Discord interaction acknowledgements run in a dedicated worker. Model, speech
and activity delivery do not share its execution loop. The durable claim occurs
before network transfer; an interrupted or unconfirmed transfer remains
uncertain and is not blindly repeated. Local failures before transfer are
recorded as `ack-not-sent:<reason>`, distinct from uncertain delivery.

## Receipt allocation candidate — 21 October 2026

An observed failed interaction had receipt-file preparation timestamps extending
past the acknowledgement deadline on One Touch. This supports a file-creation
delay; it does not conclusively reconstruct the historical network outcome or
explain all model latency. Original uncertainty is preserved.

The candidate allocates exclusive canonical response/header files on a separate
worker, ahead of intake. Eight refill credits provide a small reserve; consuming
a pair replenishes one credit, so this is a buffer rather than a work limit.
Idle polling does not allocate more files. Every pair is used once. The network
transport owns and closes consumed descriptors; shutdown joins the allocator
and closes remaining pairs. No response files or credentials are moved into a
shared workspace.

If the reserve is temporarily empty, receipt acquisition can wait only within
the remaining callback deadline. Exhaustion or allocation failure is a known
unsent failure. The deadline is recomputed before actual network transfer.
This cannot make an arbitrarily slow or unavailable disk/network meet Discord's
deadline; actual burst and restart acceptance is required.

Status: four concurrent native fixtures on One Touch passed exclusive-pair,
refill, shutdown-drain and missing-directory checks. Each produced exactly
three distinct response/header pairs. The worker build and original acknowledgement regression also passed.
The dedicated idle ACK worker was then replaced without restarting the other
five workers or interrupting the active human model run. Three actual Discord
commands (`/new`, `/history`, `/history`) completed with HTTP 204 acknowledgements
and visible final responses. Header-file modification times were approximately
691, 468 and 365 ms after durable intake; these are response-write observations,
not instrumented end-to-end latency. Sustained-stall/burst and full restart
acceptance remain open. See the dated receipt in `docs/releases/1.75/`.
The ACK repair is deployed. General conversation latency and the morning-baseline
comparison remain separate open requirements.
