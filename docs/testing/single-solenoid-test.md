# Single Solenoid Test

The included Robot suite focuses on the starter CLI commands.

## Expected coverage

- `sol_single_on <index>`  turn on index pin
- `sol_single_off <index>` turn off index pin
- `sol_single_get <index>` returns status of index pin
- `Expander Write and Read Check` returns `0x30`

## Purpose

These tests provide a fast confidence check after each build or simulation change.