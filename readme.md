# 🗃️DBSCAN in C++

This is an adaptation of [Modern C++](https://www.youtube.com/@moderncpp)'s DBSCAN implementation.

[Video link here](https://www.youtube.com/watch?v=87ogbzhXUxo)

# ⚙️Custom Arguments

You can run the program with your own custom 2D points by passing them as command-line arguments.

Each pair of numbers is interpreted as an (x, y) point. The format is:
``
DBSCAN.exe x1 y1 x2 y2 x3 y3
``

The Points {-100, 345} {189, -28.84} would be entered as such:
``
DBSCAN.exe -100 345 189 -28.84
``

# 📠footnote

The set_union() function was the only thing I really had to change, adding vector_union(), otherwise it is the same code that now prints out the data Points and their Cluster IDs.