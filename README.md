# flipper-badger

A Flipper Zero "digital badge" app: display a QR code, show contact information,
and emulate an NFC NDEF URL tag so a phone can scan it.

## Repository layout

| Path | Purpose |
|---|---|
| `flipper_badger/` | App source (FAP) — see its [README](flipper_badger/README.md). |
| `flipper-badger_vault/` | Obsidian/Tolaria vault — the project's persistent memory — see [Home](flipper-badger_vault/Home.md). |
| `nfc/` | Vendored reference copy of the official Flipper NFC app. |
| `qrcode/` | Vendored QR app (subtree of [bmatcuk/flipperzero-qrcode](https://github.com/bmatcuk/flipperzero-qrcode)). |
| `stella-green.badger` | Sample badge config; copy to `/ext/apps_data/flipper_badger/` on the SD card. |

## Quick start

1. Copy `flipper_badger.fap` into `apps/Tools/` on the Flipper's SD card.
2. Add a `.badger` config at `/ext/apps_data/flipper_badger/` — see the
   [app README](flipper_badger/README.md) for the format.

## Building

Clone this repo into `applications_user/` of a Momentum firmware tree and run:

```bash
./fbt fap_flipper_badger
```

## Project memory

`flipper-badger_vault/` tracks architecture, ADRs, progress, and gaps. Open it in
Obsidian/Tolaria; AI agents follow [`AGENTS.md`](flipper-badger_vault/AGENTS.md).

## License

MIT. The vendored QRCode library is MIT (`qrcode/LICENSE`).
