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
    // print and println keywords may have brackets (it's optional)
    print('Please enter a name: ')

    // Initialise a Person object using input()
    Person person = new Person(input())

    // Call the Person object's show function
    person.show()

    // Print some test sentences
    println (person.name + " is this person's name") + '.' // Just a test line
    println 'Bunch of gibberish. ' + (person.name) + '.' // Also just a test line
end
