// Demonstrates an object.

// Can also be `object | potato`
// The second argument is optional, and defaults to
// the name of this file, minus the extension.
object
    int | x
    int | y
end

// This is the constructor for the potato class
// Can also be `class | potato | (int x, int y)`
// Once again, the second argument is optional.
class | (int x, int y)

    // Set potato's x value
    set | self.x | x

    // Set potato's y value
    set | self.y | y

end
