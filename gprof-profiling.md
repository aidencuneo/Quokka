-- Profiling with gprof --

1. Run `gcc -g -pg src/quokka.c -o bin/quokka`

2. Run `bin/quokka` with any file argument (execute a file with this Quokka executable)

3. Locate gmon.out, and move it into `Quokka/`

4. In `Quokka/`, Run `gprof bin/quokka > profile.txt`

5. Read profile.txt
