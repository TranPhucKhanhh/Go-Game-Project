# Instructions for this repository only

These instructions apply only to `D:/project/cpp/go-game-project` and its descendants.

## Weekly reports and prompt records

- Read `.agents/skills/go-game-weekly-report/SKILL.md` when preparing reports, AI declarations, commit summaries, evidence, or submission ZIPs.
- For **every user prompt**, including questions and follow-up messages, record the original user text and start time as the first practical action using `.idea/reporting/report.py start`. Before the final response, use `finish` to record the response, end time, elapsed time, and resulting Git HEAD. See the skill's prompt-recording commands. This applies even when no report is requested.
- Record only visible conversation content, public tool outcomes, and work summaries; never internal reasoning. If a turn was interrupted, preserve its record and mark the interruption only if known. Do not invent missing timestamps or completed work. If logging starts late, say so in the timing note.
- Use Asia/Saigon (UTC+07:00) for reporting. Saturday **2026-10-24 is practical week 03**, corresponding to theory week 04. Weeks run Monday through Sunday unless the user corrects the academic calendar. Never infer the course week from old commit dates.
- Group: **Group71**. Confirmed student: **25125020 — Trần Phúc Khánh — TranPhucKhanhh**. The editable identity/configuration record is `.idea/reporting/config.json`; retain unknown authors as unresolved until confirmed.
- Trần Phúc Khánh is the sole active member. His confirmed Git aliases are listed in the configuration. Lý Tuấn Kiệt is a former teammate: omit him from current responsibilities and active-member contribution sections, while preserving original authorship wherever his commits are included. Screenshots may be left for the user to add later; such exports remain drafts without advancing a final submission checkpoint.
- `Git Commit Log Total.md/.pdf` contains only one section, `Week NN group commits (COUNT)`, and that practical week's commit entries (currently `Week 03 group commits (2)`). No per-member lists, identity/alias mappings, scope summaries or snapshot HEAD. Use `_sources/commits-week.json`, filtered by committer timestamp in Asia/Saigon from Monday 00:00 inclusive to the next Monday 00:00 exclusive. Other Git metadata and history stay internal.
- Submission contents are exactly `AI Usage Declaration.md/.pdf`, `Report.md/.pdf`, `Git Commit Log Total.md/.pdf`, and `screenshots/`. Keep prose short and the AI declaration summarized. Do not include HTML, slides, separate chat/evidence reports, videos or internal logs in the submission.
- Omit validation and remaining-work sections/notes from `Report` and `AI Usage Declaration`; keep verification records internal.
- Write report prose in **English**. Preserve names and original Git messages. Exact prompts/replies and timing remain internal and outside the submission. Conversation with the user can remain Vietnamese.
- All reports, transcripts, commit snapshots, app screenshots, demo videos, temporary files, test artifacts for this workflow, and other task scratch material belong in `.idea/`, organized by week. Preserve existing IDE files. Leave game source/build/assets where they belong.
- Preserve `.idea/reporting/baseline.json`: it is the initial tracking checkpoint, not the beginning of the project. Do not replace it with a newer HEAD. Store later report checkpoints separately.
- `.idea/` is already Git-ignored. Do not force-add private logs, reports, or temporary files. Do not create commits merely to track a prompt.
- Refactoring or code cleanup is unnecessary unless required to implement a requested feature or fix.

Repository instructions are stored in `.idea/AGENTS.md` at the user's request. Read this file explicitly when working on this project; it is not a root-level automatic discovery file. Only this instruction file is tracked from `.idea/`; reports, private logs, IDE settings and scratch files remain ignored. The reporting skill remains in `.agents/skills/go-game-weekly-report/SKILL.md`.
