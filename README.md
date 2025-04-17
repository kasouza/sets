# sets
Tried out a few different implementations of set intersection in a few different languages

## Methods
### Method 1 (slowest)
Simply loop over the two lists of items and save the ones that are in both in a third list

### Method 2 (fast)
Add all the elements of the smaller list to a hashset (or hashmap), then iterate through the larger list, checking if each element is in the hashset, if it is, add to the resulting intersecition

Here the result might be a list (or array or something like that) or a set. Adding to a list is considerably faster, so if either the input likes are guaranteed to have unique elements or you just don't care if the result has duplicates, then you should use a list/array, otherwise use a set.

### Method 3 (faster?)
Consider two lists `a` and `b`
    - 1. Sort both lists
    - 2. Iterate through both of them at the same time, using 2 indices `i` and `j`. 
        - If `a[i]` is less than `b[j]` then increment `i`
        - If `a[i]` is greater than `b[j]` then increment `j`
        - If `a[i]` is equal to `b[j]` then add this element to your result list

This method is REALLY fast BUT, only if the lists `a` and `b` are already sorted to start with, otherwise the Method 2 is faster, simply because sorting the lists takes way too long

## Languages
In order of how well they performed in my tests

- C         -> The GOAT, but at at cost of writing a lot of code. Fastest implementation I came up with used the second method in this language. Really cool, but it's only faster than C++ if you actually try to optimize the code a little bit, otherwise using the C++ std containers is just faster
- C++       -> Probably the best overall, very easy to write and great performance, similar to C (and gives you room to make it better, for example by just using the C version). You can make you life really easy by using the STL containers, especially when compared to implementing it all in raw C. In fact there's not even a need to implement it in C++, the `std::set_intersection` function is right there just waiting for you
- Python??? -> For some reason python is actually REALLY fast for this, eitheir because the C code it calls is really optimised, maybe even using some SIMD stuff OR I just completely butchered the implementation and it's not actually working
- Go        -> The cleanest looking version for me. I really like go. Good performance, but not THAT great if you plan on doing a lot of singled-threaded heavy calculations like this, works better for async stuff like APIs and such
- JS        -> Complete garbage trash I hate it. Nah just kidding. It's actually super easy to write too and actually has a pretty decent performance
    - Node      -> V8 does a great job at turning javascript into something usefull. Eats a lot of RAM though
    - Deno      -> The same as node (it also uses V8)
    - Bun       -> A little bit faster than node, but use a lot less ram

## Final considerations
So just to sum it up, C using the second method is overall the fastest, with C using the third method being the fastest for specific scenarios (pre-sorted lists).
But I would say that actually C++ wins this one because of how easy it was to write some great perfoming code. Means you can sit down and actually get stuff done in an hour, instead of spending 3 hours trying to debug something written in C.
The other ones I would not be my first choice if I had to do anything heavy-weight.

So in the end, even though I really love writing code in C (and will actually use the C implementation in a project), the best language for the job would probably be C++.
