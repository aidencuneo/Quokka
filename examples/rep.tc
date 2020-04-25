// Demonstrates string replacement

main:
    // Create and print the original string:
    string  | str  | 'The first <animal> looked at the second <animal>.'
    println | str

    // Create new string with all '<animal>' items replaced with 'cat'
    string  | rep  | '<animal>'
    string  | with | 'cat'
    string  | nstr | stringreplace | (str, rep, with)

    // Print new string
    println | nstr
end
