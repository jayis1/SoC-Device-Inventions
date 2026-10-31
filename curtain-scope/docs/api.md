# USB CDC protocol

Host sends ASCII `START\n`. Device clears previous capture and responds `index,us,status\n`. On five falling edges it emits five rows: index 0..4 and microseconds relative to gate 0. It then sends `END\n`. Missing gates produce no completed capture; reset the board and retry. Gate order is physical left-to-right when viewed from the film plane. Timestamp order therefore estimates curtain passage skew across that plane, not shutter duration at a single point. No host parser dependency is needed; output is CSV-compatible.
