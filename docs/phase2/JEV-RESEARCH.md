> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Jev review — 16 October 2026

Decision: do not integrate Jev into Echo Phase Two. The requested condition is open, freely available local execution. The official model catalogue provides hosted API access; the customer agreement defines TypeSafe-hosted services. No downloadable Jev weights or self-hosted model release was found in the official documentation or GitHub organization. Open SDKs are not open model weights.

The newest documented version is Jev 1.14, released 14 October, exposed through the preview alias; the stable latest alias still points to 1.13. It predicts typed choices, scores and yes/no probabilities in parallel rather than autoregressively writing an answer. This is useful learned classification, not merely a hand-written if/then table; application code still composes the decisions. Public material does not establish the full architecture or training corpus.

The launch Doom demo uses structured text game state, not pixels, and reports ten queries per second. It demonstrates low-latency action selection with a game-state harness. It does not establish a general visual robotics controller or show that every LLM implementation is incapable of real-time action. Type correctness prevents invalid option types; it does not guarantee a correct decision. The same broad architecture—fast local action selection alongside slower planning—could suit Echo, but Jev does not meet the user's local/open requirement. No API account, download, dependency or Phase Two implementation scope was added.

Sources inspected:
- https://docs.typesafe.ai/models
- https://docs.typesafe.ai/introduction
- https://typesafe.ai/blog/introducing-system-one-models-and-jev
- https://typesafe.ai/legal/mca
- https://github.com/typesafe-ai
