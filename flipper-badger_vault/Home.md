---
type: Project
status: Active
stage: Phase 5 complete
repo: flipper-badger
license: MIT
related_to:
  - "[[overview|Architecture Overview]]"
  - "[[current-state|Current State]]"
  - "[[gap-registry|Gap Registry]]"
tags:
  - flipper-badger
  - flipper-zero
  - nfc
  - qr
---

# flipper-badger — Project Home

Dashboard and map of content for **flipper-badger**, a Flipper Zero "digital badge"
app that displays a QR code, shows contact information on demand, and emulates an
NFC NDEF URL tag so it can be scanned by phones.

This vault is the project's persistent memory for humans and AI agents. Start with
`progress/current-state.md`, then navigate the category views.

## Status

- **Stage:** `Phase 5 complete` — app builds to `flipper_badger.fap`.
- **Language:** C (Flipper Zero FAP), built against Momentum firmware.
- **Code:** `Momentum-Firmware/applications_user/flipper_badger/` (see
  `AGENTS.md` for the absolute path).
- **Open gaps:** see [[gap-registry|Gap Registry]].

## Categories

| Category | View | Notes |
|---|---|---|
| Architecture | [[overview|Architecture Overview]] | `architecture/` |
| Components | `components/` | [[flipper-badger-app|App Core]], [[badger-config|Config Loader]], [[ndef-url|NDEF URL]], [[badger-nfc|NFC Emulation]], [[qrcode|QR Code (vendored)]] |
| Decisions | `decisions/` | ADR-0001 … ADR-0005 |
| Source files | `files/` | per `.c`/`.h` indexes |
| Progress | [[current-state|Current State]], [[phases|Phases]], [[changelog|Changelog]], [[worklog|Worklog]], [[sessions|Sessions]] | `progress/` |
| References | `references/` | [[qrcode-app-analysis|QR App Analysis]], [[nfc-app-analysis|NFC App Analysis]], [[ndef-spec|NDEF Spec]], [[momentum-sdk|Momentum SDK]], [[build-guide|Build Guide]] |
| Gaps | [[gap-registry|Gap Registry]] | [high](gaps-high) · [medium](gaps-medium) · [low](gaps-low) · [feature](gaps-feature) |

## Quick links

- [[overview|Architecture Overview]]
- [[config-format|Config File Format]]
- [[qr-rendering|QR Rendering]]
- [[ndef-url|NDEF URL Encoding]]
- [[nfc-emulation|NFC Emulation]]
- [[current-state|Current State]]
- [[phases|Phases]]
- [[gap-registry|Gap Registry]]
- [[0001-single-view-app|ADR-0001 Single-view app]]

## How to use this vault

- **Humans:** open it in Obsidian or Tolaria and browse the saved views.
- **Agents:** follow the workflow in `AGENTS.md`; read [[current-state|Current State]] first and
  append to [[worklog|Worklog]] after every session.
