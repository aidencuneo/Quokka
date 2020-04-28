// Ignore this file, I was in the middle of writing it

object | StringList
    string[] | value
    int      | length

    def | void   | append   | string item
    def | string | toString | 
end

class | StringList | ()

end

main:
    println | 'Started.'

    new | StringList | mylist | ()
    println | mylist.toString | ()

    println | 'Success.'
end
