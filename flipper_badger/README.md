# flipper-badger

A Flipper Zero "digital badge" app: display a QR code, show contact information,
and emulate an NFC NDEF URL tag.

- **QR code** — the `Url` field rendered as a QR code (auto version/ECC).
- **Contact card** — press **Right** to see name/title/phone/email/website/note.
- **NFC** — press **OK** to emulate an NTAG213 carrying the `Url` as an NDEF URL;
  phones tap the back of the Flipper to open it. **Back** stops emulation.

## Installation

1. Copy `flipper_badger.fap` into `apps/Tools/` on the SD card.
2. Create a config file at `/ext/apps_data/flipper_badger/<name>.badger`:

```
Filetype: Badger
Version: 1
Url: https://example.com
Name: John Smith
Title: Engineer
Phone: +18005551212
Email: john@example.com
Website: https://example.com
Note: Hi, I'm a badge
```

`Url` is required (it drives the QR and the NFC tag). All other fields are optional.

## Building

Clone this into `applications_user/` of the firmware and run:

```bash
./fbt fap_flipper_badger
```

## License

MIT. Includes the vendored [QRCode](https://github.com/ricmoo/QRCode) library (MIT).
