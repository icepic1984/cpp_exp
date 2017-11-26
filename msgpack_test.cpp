#include <msgpack.hpp>
#include <tuple>
#include <string>
#include <iostream>
#include <sstream>
#include <boost/callable_traits/args.hpp>

int add(int a, int b) {
    return a + b;
}

// Packs arbitary number of arguments into msgpack buffer
template <typename... Args>
msgpack::sbuffer pack(Args&&... args) 
{
    msgpack::sbuffer buffer;
    msgpack::pack(buffer, std::make_tuple(std::forward<Args>(args)...));
    return buffer;
}

template <typename T>
struct unpack_helper;

template <typename... Args>
struct unpack_helper<std::tuple<Args...>> {
    static constexpr auto unpack(const msgpack::sbuffer& buffer){
        std::tuple<Args...> tmp;
            msgpack::object_handle oh =
                msgpack::unpack(buffer.data(), buffer.size());

            msgpack::object obj = oh.get();
            obj.convert(tmp);
            return tmp;
    }
};

// Unpacks buffer into std::tuple 
template<typename T>
constexpr auto unpack(const msgpack::sbuffer& buffer)
{return unpack_helper<T>::unpack(buffer);}




//http://www.boost.org/doc/libs/develop/libs/callable_traits/doc/html/callable_traits/reference.html#callable_traits.reference.ref_args

    
int main(int argc, char *argv[])
{
    // Client side, pack parameter into std::tuple
    auto packed = pack(10,20);

    // Send to server
    // ....
    //...
    

    // On Server side
    // Retrieve registered function pointer
    // ....
    // 
    // Get tuple with types of argument
    using Fn = decltype(add);
    using args_t = boost::callable_traits::args_t<Fn>;

    // Unpack retrieved message into tuple
    auto unpacked = unpack<args_t>(packed);

    // Unpack tuple into function call
    auto result = std::apply(add, unpacked);
    std::cout << result << std::endl;
    

    
    // convert msgpack::object instance into the original type.
    // if the type is mismatched, it throws msgpack::type_error exception.
    //msgpack::type::tuple<int, bool, std::string> dst;
    //deserialized.convert(dst);
    
    return 0;
}
