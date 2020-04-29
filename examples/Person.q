// Declares the boundaries of the Person class
object

    string name
    string id

    def void show // Functions are included (only the head)

end


// Defines the constructor for the Person class
class(string name)

    self.name   = name
    integer num = name.length * 5 + 1
    self.id     = new string(num)

    // Functions are included here as well (full functions)
    def void show
        println self.name + ', ID: ' + self.id
    end

end


main:
    Person john = new Person('John Smith')
    john.show()
end
