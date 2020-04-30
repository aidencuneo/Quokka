// Declares the boundaries of the Person class
object

    string name
    string id

    def void show // Functions are included (only the head)

end


// Defines the constructor for the Person class
class(string name)

    self.name = name
    self.id   = new string(name.length * 5 + 1)

    // Functions are included here as well (full functions)
    def void show
        println self.name + ', ID: ' + self.id
    end

end


main:
    bool boolean = false
    println boolean

    string aiden = 'aiden'
    Person john = new Person(readchar())
    john.show()
    println (john.name + " is this person's name") + 'dskjcndskjcsd'
end
