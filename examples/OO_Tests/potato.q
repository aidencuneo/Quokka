// Demonstrates an object.

// This is the object for the Potato class.
// Can also be `object | Potato`, but the
// object name is optional, and defaults
// to the current filename (minus `.q`).
object
    int | x
    int | y

    def | void | summary
end

// This is the constructor for the Potato class.
// Can also be `class | Potato | (int x, int y)`
// Once again, the name is optional.
class | (int x, int y)

    set | self.x | x
    set | self.y | y

    def | void  | summary
        print   | self.x
        print   | ','
        println | self.y
    end

end

// Just another ALMOST identical class to show
// that Quokka can differentiate between multiple
// classes with IDENTICAL values inside them.
object | Point
    int | x
    int | y

    def | void | summary
end

class | Point | (int x, int y)

    set | self.x | x
    set | self.y | y

    def | void  | summary
        print   | self.x
        print   | ','
        println | self.y
    end

end
