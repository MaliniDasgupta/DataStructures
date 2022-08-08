// TODO: Implement the methods of class Array here or in the header file.
template <typename T, std::size_t Length>
Array<T, Length>::Array()
{
    ar = new T[Length];
    len = Length;
}

template <typename T, std::size_t Length>
Array<T,Length>::~Array()
{
    delete[] ar;
}

template <typename T, std::size_t Length>
Array<T,Length>::Array(const Array& other)
{
    len = other.size();
    ar = new T[len];
    for(unsigned i=0;i<len;i++){
        ar[i] = other.at(i);
    }
}

template <typename T, std::size_t Length> 
Array<T, Length>& Array<T,Length>::operator=(const Array& other)
{
    len = other.size();
    delete[] ar;
    ar = new T[len];
    for(unsigned i=0;i<len;i++){
        ar[i] = other.at(i);
    }

    return (*this);
}
// For your convenience, I show you a bit of what the definition of the size()
// method looks like below.
template <typename T, std::size_t Length>
std::size_t Array<T, Length>::size() const
{
    // TODO: Implement.
    return len;

}

template <typename T, std::size_t Length>
const T& Array<T, Length>::front() const
 {
     return ar[0];
 }

template <typename T, std::size_t Length>
const T& Array<T, Length>::back() const
 {
     return ar[len-1];
 }

template <typename T, std::size_t Length>
T& Array<T, Length>::at(std::size_t index)
{
    if(index<0 || index>=len){
        throw std::out_of_range{"Index out of range."};
    }
    return ar[index];
}

template <typename T, std::size_t Length>
const T& Array<T, Length>::at(std::size_t index) const
{
    if(index<0 || index>=len){
        throw std::out_of_range{"Index out of range."};
    }
    return ar[index];
}

template <typename T, std::size_t Length>
bool Array<T, Length>::operator==(const Array& other) const
{
    for(unsigned i=0;i<len;i++){
        if(ar[i]!=other.at(i)){
            return 0;
        }
    }
    return 1;
}

template <typename T, std::size_t Length>
bool Array<T, Length>::operator!=(const Array& other) const
{
    int check = 0;
    for(unsigned i=0;i<len;i++){
        if(ar[i]!=other.at(i)){
            check++;
        }
    }
    return check;
}

/**
 * Here, Index must come before T and Length in the `template < ... >` line.
 * I'm not sure why; I just know the compiler wants it that way.
 */
template <std::size_t Index, typename T, std::size_t Length>
T& get(Array<T, Length>& array)
{
    // TODO: Provide a static assertion that causes compilation to fail
    // if the given index is out of range.

    // TODO: Retrieve the element.
    static_assert(Index>=0 && Index<Length, "Index out of bounds.");
    return array.at(Index);


}

// TODO: Implement the const overload of get(), either here or in the header
// file.
template <std::size_t Index, typename T, std::size_t Length>
const T& get(const Array<T, Length>& array)
{
    static_assert(Index>=0, "Index out of bounds.");
    static_assert(Index<Length, "Index out of bounds.");
    return array.at(Index);
}

template <typename T, std::size_t Length>
std::ostream& operator<<(std::ostream& os, const Array<T, Length>& array)
{
    for(unsigned i=0;i<Length;i++){
        os<<array.at(i)<<' ';
    }
    os<<"\n";
    return os;
}


// TODO: Implement operator<<, either here or in the header file.
