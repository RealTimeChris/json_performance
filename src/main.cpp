#include "glaze/glaze.hpp"
#include "glaze/glaze_exceptions.hpp"
#include <numeric>

[[maybe_unused]] constexpr std::string_view json_whitespace = R"(
{
   "fixed_object": {
      "int_array": [0, 1, 2, 3, 4, 5, 6],
      "float_array": [0.1, 0.2, 0.3, 0.4, 0.5, 0.6],
      "double_array": [3288398.238, 233e22, 289e-1, 0.928759872, 0.22222848, 0.1, 0.2, 0.3, 0.4]
   },
   "fixed_name_object": {
      "name0": "James",
      "name1": "Abraham",
      "name2": "Susan",
      "name3": "Frank",
      "name4": "Alicia"
   },
   "another_object": {
      "string": "here is some text",
      "another_string": "Hello World",
      "escaped_text": "{\"some key\":\"some string value\"}",
      "boolean": false,
      "nested_object": {
         "v3s": [[0.12345, 0.23456, 0.001345],
                  [0.3894675, 97.39827, 297.92387],
                  [18.18, 87.289, 2988.298]],
         "id": "298728949872"
      }
   },
   "string_array": ["Cat", "Dog", "Elephant", "Tiger"],
   "string": "Hello world",
   "number": 3.14,
   "boolean": true,
   "another_bool": false
}
)";

constexpr std::string_view json_minified = R"({"fixed_object":{"int_array":[0,1,2,3,4,5,6],"float_array":[0.1,0.2,0.3,0.4,0.5,0.6],"double_array":[3288398.238,2.33e+24,28.9,0.928759872,0.22222848,0.1,0.2,0.3,0.4]},"fixed_name_object":{"name0":"James","name1":"Abraham","name2":"Susan","name3":"Frank","name4":"Alicia"},"another_object":{"string":"here is some text","another_string":"Hello World","escaped_text":"{\"some key\":\"some string value\"}","boolean":false,"nested_object":{"v3s":[[0.12345,0.23456,0.001345],[0.3894675,97.39827,297.92387],[18.18,87.289,2988.298]],"id":"298728949872"}},"string_array":["Cat","Dog","Elephant","Tiger"],"string":"Hello world","number":3.14,"boolean":true,"another_bool":false})";

#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include <format>

struct fixed_object_t
{
   std::vector<int> int_array;
   std::vector<float> float_array;
   std::vector<double> double_array;
};


struct fixed_name_object_t
{
   std::string name0{};
   std::string name1{};
   std::string name2{};
   std::string name3{};
   std::string name4{};
};


struct nested_object_t
{
   std::vector<std::array<double, 3>> v3s{};
   std::string id{};
};


struct another_object_t
{
   std::string string{};
   std::string another_string{};
   std::string escaped_text{};
   bool boolean{};
   nested_object_t nested_object{};
};


struct obj_t
{
   fixed_object_t fixed_object{};
   fixed_name_object_t fixed_name_object{};
   another_object_t another_object{};
   std::vector<std::string> string_array{};
   std::string string{};
   double number{};
   bool boolean{};
   bool another_bool{};
};


template <>
struct glz::meta<fixed_object_t> {
   using T = fixed_object_t;
   static constexpr auto value = object(
      &T::int_array,
      &T::float_array,
      &T::double_array
   );
};

template <>
struct glz::meta<fixed_name_object_t> {
   using T = fixed_name_object_t;
   static constexpr auto value = object(
      &T::name0,
      &T::name1,
      &T::name2,
      &T::name3,
      &T::name4
   );
};

template <>
struct glz::meta<nested_object_t> {
   using T = nested_object_t;
   static constexpr auto value = object(
      &T::v3s,
      &T::id
   );
};

template <>
struct glz::meta<another_object_t> {
   using T = another_object_t;
   static constexpr auto value = object(
      &T::string,
      &T::another_string,
      &T::escaped_text,
      &T::boolean,
      &T::nested_object
   );
};

template <>
struct glz::meta<obj_t> {
   using T = obj_t;
   static constexpr auto value = object(
      &T::fixed_object,
      &T::fixed_name_object,
      &T::another_object,
      &T::string_array,
      &T::string,
      &T::number,
      &T::boolean,
      &T::another_bool
   );
};

// for testing large, flat documents and out of sequence reading
template <bool backward>
struct abc_t
{
   std::vector<int64_t> a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z;
   bool initialized = init();
   
   bool init() {
      auto fill = [](auto& v) {
         v.resize(1000);
         std::iota(v.begin(), v.end(), 0);
      };
      
      fill(a); fill(b); fill(c);
      fill(d); fill(e); fill(f);
      fill(g); fill(h); fill(i);
      fill(j); fill(k); fill(l);
      fill(m); fill(n); fill(o);
      fill(p); fill(q); fill(r);
      fill(s); fill(t); fill(u);
      fill(v); fill(w); fill(x);
      fill(y); fill(z);
      return true;
   }
};

template <>
struct glz::meta<abc_t<false>>
{
   using T = abc_t<false>;
   static constexpr auto value = object(&T::a,&T::b,&T::c,&T::d,&T::e,&T::f,&T::g,&T::h,&T::i,&T::j,&T::k,&T::l,&T::m,&T::n,
                                        &T::o,&T::p,&T::q,&T::r,&T::s,&T::t,&T::u,&T::v,&T::w,&T::x,&T::y,&T::z);
};

template <>
struct glz::meta<abc_t<true>>
{
   using T = abc_t<true>;
   static constexpr auto value = object(&T::z,&T::y,&T::x,&T::w,&T::v,&T::u,&T::t,&T::s,&T::r,&T::q,&T::p,&T::o,&T::n,
                                        &T::m,&T::l,&T::k,&T::j,&T::i,&T::h,&T::g,&T::f,&T::e,&T::d,&T::c,&T::b,&T::a);
};

#ifdef NDEBUG
static constexpr size_t iterations = 1'000'000;
static constexpr size_t iterations_abc = 10'000;
#else
static constexpr size_t iterations = 100'000;
static constexpr size_t iterations_abc = 1'000;
#endif

// We scale all speeds by the minified JSON byte length, so that libraries which do not efficiently write JSON do not get an unfair advantage
// We want to know how fast the libraries will serialize/deserialize with repsect to one another
size_t minified_byte_length{};

struct results
{
   std::string_view name{};
   std::string_view url{};
   size_t iterations{};
   
   std::optional<size_t> json_byte_length{};
   std::optional<double> json_read{};
   std::optional<double> json_write{};
   std::optional<double> json_roundtrip{};
   
   std::optional<size_t> binary_byte_length{};
   std::optional<double> binary_write{};
   std::optional<double> binary_read{};
   std::optional<double> binary_roundtrip{};
   
   void print(bool use_minified = true)
   {
      if (json_roundtrip) {
         std::cout << name << " json roundtrip: " << *json_roundtrip << " s\n";
      }
      
      if (json_byte_length) {
         std::cout << name << " json byte length: " << *json_byte_length << '\n';
      }
      
      if (json_write) {
         if (json_byte_length) {
            const auto byte_length = use_minified ? minified_byte_length : *json_byte_length;
            const auto MBs = iterations * byte_length / (*json_write * 1048576);
            std::cout << name << " json write: " << *json_write << " s, " << MBs << " MB/s\n";
         }
         else {
            std::cout << name << " json write: " << *json_write << " s\n";
         }
      }
      
      if (json_read) {
         if (json_byte_length) {
            const auto byte_length = use_minified ? minified_byte_length : *json_byte_length;
            const auto MBs = iterations * byte_length / (*json_read * 1048576);
            std::cout << name << " json read: " << *json_read << " s, " << MBs << " MB/s\n";
         }
         else {
            std::cout << name << " json read: " << *json_read << " s\n";
         }
      }
      
      if (binary_roundtrip) {
         std::cout << '\n';
         std::cout << name << " binary roundtrip: " << *binary_roundtrip << " s\n";
      }
      
      if (binary_byte_length) {
         std::cout << name << " binary byte length: " << *binary_byte_length << '\n';
      }
      
      if (binary_write) {
         if (binary_byte_length) {
            const auto MBs = iterations * *binary_byte_length / (*binary_write * 1048576);
            std::cout << name << " binary write: " << *binary_write << " s, " << MBs << " MB/s\n";
         }
         else {
            std::cout << name << " binary write: " << *binary_write << " s\n";
         }
      }
      
      if (binary_read) {
         if (binary_byte_length) {
            const auto MBs = iterations * *binary_byte_length / (*binary_read * 1048576);
            std::cout << name << " binary read: " << *binary_read << " s, " << MBs << " MB/s\n";
         }
         else {
            std::cout << name << " binary read: " << *binary_read << " s\n";
         }
      }
      
      std::cout << "\n---\n" << std::endl;
   }
   
   std::string json_stats(bool use_minified = true) const {
      static constexpr std::string_view s = R"(| [**{}**]({}) | **{}** | **{}** | **{}** |)";
      const std::string roundtrip = json_roundtrip ? std::format("{:.2f}", *json_roundtrip) : "N/A";
      if (json_byte_length) {
         const auto byte_length = use_minified ? minified_byte_length : *json_byte_length;
         const std::string write = json_write ? std::format("{}", static_cast<size_t>(iterations * byte_length / (*json_write * 1048576))) : "N/A";
         const std::string read = json_read ? std::format("{}", static_cast<size_t>(iterations * byte_length / (*json_read * 1048576)))  : "N/A";
         return std::format(s, name, url, roundtrip, write, read);
      }
      else {
         const std::string write = json_write ? std::format("{:.2f}", *json_write)  : "N/A";
         const std::string read = json_read ? std::format("{:.2f}", *json_read)  : "N/A";
         return std::format(s, name, url, roundtrip, write, read);
      }
   }
   
   std::string json_stats_read(bool use_minified = true) const {
      static constexpr std::string_view s = R"(| [**{}**]({}) | **{}** |)";
      if (json_byte_length) {
         const auto byte_length = use_minified ? minified_byte_length : *json_byte_length;
         const std::string read = json_read ? std::format("{}", static_cast<size_t>(iterations * byte_length / (*json_read * 1048576)))  : "N/A";
         return std::format(s, name, url, read);
      }
      else {
         const std::string read = json_read ? std::format("{:.2f}", *json_read)  : "N/A";
         return std::format(s, name, url, read);
      }
   }
};

template <class T>
inline bool is_valid_write(const std::string& buffer, const std::string& library_name) {
   T obj{};
   
   glz::ex::read_json(obj, json_minified);
   
   std::string reference = glz::write_json(obj).value();
   
   obj = {};
   glz::ex::read_json(obj, buffer);
   
   std::string compare = glz::write_json(obj).value();
   
   if (reference != compare) {
      std::cout << "Invalid write for library: " << library_name << std::endl;
      return false;
   }
   
   return true;
}

template <glz::opts Opts>
auto glaze_test()
{
   std::string buffer{ json_minified };
   
   obj_t obj;
   
   auto t0 = std::chrono::steady_clock::now();
   
   for (size_t i = 0; i < iterations; ++i) {
      if (glz::read<Opts>(obj, buffer)) {
         std::cout << "glaze error!\n";
         break;
      }
      if (glz::write_json(obj, buffer)) {
         std::cout << "glaze error!\n";
         break;
      }
   }
   
   auto t1 = std::chrono::steady_clock::now();
   
   results r{ Opts.minified ? "Glaze (.minified)" : "Glaze", "https://github.com/stephenberry/glaze", iterations };
   r.json_roundtrip = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;
   
   // write performance
   t0 = std::chrono::steady_clock::now();
   
   for (size_t i = 0; i < iterations; ++i) {
      if (glz::write<Opts>(obj, buffer)) {
         std::cout << "glaze error!\n";
         break;
      }
   }
   
   t1 = std::chrono::steady_clock::now();
   
   r.json_byte_length = buffer.size();
   minified_byte_length = *r.json_byte_length;
   r.json_write = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;
   
   is_valid_write<obj_t>(buffer, "Glaze");
   
   // read performance
   
   t0 = std::chrono::steady_clock::now();
   
   for (size_t i = 0; i < iterations; ++i) {
      if (glz::read_json(obj, buffer)) {
         std::cout << "glaze error!\n";
         break;
      }
   }
   
   t1 = std::chrono::steady_clock::now();
   
   r.json_read = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;
   
   // binary write performance
   
   t0 = std::chrono::steady_clock::now();
   
   for (size_t i = 0; i < iterations; ++i) {
      if (glz::write_beve(obj, buffer)) {
         std::cout << "glaze error!\n";
         break;
      }
   }
   
   t1 = std::chrono::steady_clock::now();
   
   r.binary_byte_length = buffer.size();
   r.binary_write = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;
   
   // binary read performance
   
   t0 = std::chrono::steady_clock::now();
   
   for (size_t i = 0; i < iterations; ++i) {
      if (glz::read_beve(obj, buffer)) {
         std::cout << "glaze error!\n";
         break;
      }
   }
   
   t1 = std::chrono::steady_clock::now();
   
   r.binary_read = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;
   
   // binary round trip
   
   t0 = std::chrono::steady_clock::now();
   
   for (size_t i = 0; i < iterations; ++i) {
      if (glz::read_beve(obj, buffer)) {
         std::cout << "glaze error!\n";
         break;
      }
      if (glz::write_beve(obj, buffer)) {
         std::cout << "glaze error!\n";
         break;
      }
   }
   
   t1 = std::chrono::steady_clock::now();
   
   r.binary_roundtrip = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;
   
   r.print();
   
   return r;
}

auto glaze_abc_test()
{
   std::string buffer = glz::write_json(abc_t<true>{}).value();
   
   results r{ "Glaze", "https://github.com/stephenberry/glaze", iterations_abc };
   
   // read performance
   
   abc_t<false> obj{};
   
   auto t0 = std::chrono::steady_clock::now();
   
   for (size_t i = 0; i < iterations_abc; ++i) {
      if (glz::read_json(obj, buffer)) {
         std::cout << "glaze error!\n";
         break;
      }
   }
   
   auto t1 = std::chrono::steady_clock::now();
   
   r.json_byte_length = buffer.size();
   r.json_read = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;
      
   r.print(false);
   
   return r;
}

#include <jsonifier>

template <>
struct jsonifier::core<fixed_object_t> {
    using T = fixed_object_t;
    static constexpr auto parseValue = createValue<
        &T::int_array,
        &T::float_array,
        &T::double_array
    >();
};

template <>
struct jsonifier::core<fixed_name_object_t> {
    using T = fixed_name_object_t;
    static constexpr auto parseValue = createValue<
        &T::name0,
        &T::name1,
        &T::name2,
        &T::name3,
        &T::name4
    >();
};

template <>
struct jsonifier::core<nested_object_t> {
    using T = nested_object_t;
    static constexpr auto parseValue = createValue<
        & T::v3s,
        & T::id
   > ();
};

template <>
struct jsonifier::core<another_object_t> {
    using T = another_object_t;
    static constexpr auto parseValue = createValue <
        & T::string,
        & T::another_string,
        & T::escaped_text,
        & T::boolean,
        & T::nested_object
   > ();
};

template <>
struct jsonifier::core<obj_t> {
    using T = obj_t;
    static constexpr auto parseValue = createValue <
        & T::fixed_object,
        & T::fixed_name_object,
        & T::another_object,
        & T::string_array,
        & T::string,
        & T::number,
        & T::boolean,
        & T::another_bool
   > ();
};

template <>
struct jsonifier::core<abc_t<false>> {
    using T = abc_t<false>;
    static constexpr auto parseValue = createValue<
        & T::a, & T::b, & T::c, & T::d, & T::e, & T::f, & T::g, & T::h, & T::i, & T::j, & T::k, & T::l, & T::m, & T::n,
        & T::o, & T::p, & T::q, & T::r, & T::s, & T::t, & T::u, & T::v, & T::w, & T::x, & T::y, & T::z
> ();
};

template <>
struct jsonifier::core<abc_t<true>> {
    using T = abc_t<true>;
    static constexpr auto parseValue = createValue<
        & T::z, & T::y, & T::x, & T::w, & T::v, & T::u, & T::t, & T::s, & T::r, & T::q, & T::p, & T::o, & T::n,
        & T::m, & T::l, & T::k, & T::j, & T::i, & T::h, & T::g, & T::f, & T::e, & T::d, & T::c, & T::b, & T::a
> ();
};

auto jsonifier_abc_test()
{
    jsonifier::jsonifier_core<> jsonifierParser{};
    std::string buffer{};
    jsonifierParser.serializeJson(abc_t<true>{}, buffer);

    results r{ "jsonifier", "https://github.com/nihilai-collective/Jsonifier", iterations_abc };

    // read performance

    abc_t<false> obj{};

    auto t0 = std::chrono::steady_clock::now();

    for (size_t i = 0; i < iterations_abc; ++i) {
        auto error = jsonifierParser.parseJson<jsonifier::parse_options{ .minified = true }>(obj, buffer);
        if (!error) {
            std::cout << "jsonifier error!\n";
            break;
        }
    }

    auto t1 = std::chrono::steady_clock::now();

    r.json_byte_length = buffer.size();
    r.json_read = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;

    r.print(false);

    return r;
}

auto jsonifier_test()
{
    jsonifier::jsonifier_core<> jsonifierParser{};
    std::string buffer{ json_minified };
    obj_t obj;
    auto t0 = std::chrono::steady_clock::now();
    for (size_t i = 0; i < iterations; ++i) {
        auto error = jsonifierParser.parseJson<jsonifier::parse_options{ .knownOrder = true, .minified = true }>(obj, buffer);
        if (!error) {
            std::cout << "jsonifier error: " << jsonifierParser.getErrors()[0] << '\n';
        }
        buffer.clear();
        jsonifierParser.serializeJson(obj, buffer);
    }
    auto t1 = std::chrono::steady_clock::now();

    results r{ "jsonifier", "https://github.com/nihilai-collective/Jsonifier", iterations };
    r.json_roundtrip = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;

    // write performance

    t0 = std::chrono::steady_clock::now();

    for (size_t i = 0; i < iterations; ++i) {
        buffer.clear();
        jsonifierParser.serializeJson(obj, buffer);
    }

    t1 = std::chrono::steady_clock::now();

    r.json_byte_length = buffer.size();
    r.json_write = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;

    is_valid_write<obj_t>(buffer, "jsonifier");

    // read performance
    t0 = std::chrono::steady_clock::now();

    for (size_t i = 0; i < iterations; ++i) {
        auto error = jsonifierParser.parseJson<jsonifier::parse_options{ .knownOrder = true, .minified = true }>(obj, buffer);
        if (!error) {
            std::cout << "jsonifier error: " << jsonifierParser.getErrors()[0] << '\n';
        }
    }

    t1 = std::chrono::steady_clock::now();

    r.json_read = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;

    r.print();

    return r;
}

#include "simdjson.h"

// Note: the on demand parser does not allow multiple instances of the same key with different data specified
// Note: we must use find_field_unordered if keys can be missing, because find_field will iterate past keys that we might want to parse

struct on_demand {
   bool read_in_order(obj_t& obj, const simdjson::padded_string &json);
private:
   simdjson::ondemand::parser parser{};
};


bool on_demand::read_in_order(obj_t& obj, const simdjson::padded_string &json) {
   using namespace simdjson;
  auto doc = parser.iterate(json);
   if (auto fixed_object = doc.find_field_unordered("fixed_object"); fixed_object.error() == SUCCESS) {
      if (auto int_array = fixed_object.find_field_unordered("int_array"); int_array.error() == SUCCESS) {
         obj.fixed_object.int_array.clear();
         for (int64_t x : int_array) { obj.fixed_object.int_array.emplace_back(x); }
      }
      
      if (auto float_array = fixed_object.find_field_unordered("float_array"); float_array.error() == SUCCESS) {
         obj.fixed_object.float_array.clear();
         // doesn't have a direct float conversion
         for (double x : float_array) { obj.fixed_object.float_array.emplace_back(static_cast<float>(x)); }
      }
      
      if (auto double_array = fixed_object.find_field_unordered("double_array"); double_array.error() == SUCCESS) {
         obj.fixed_object.double_array.clear();
         for (double x : double_array) { obj.fixed_object.double_array.emplace_back(x); }
      }
   }
   
   if (auto fixed_name_object = doc.find_field_unordered("fixed_name_object"); fixed_name_object.error() == SUCCESS) {
      if (auto name0 = fixed_name_object.find_field_unordered("name0"); name0.error() == SUCCESS) {
         obj.fixed_name_object.name0 = std::string_view(name0);
      }
      if (auto name1 = fixed_name_object.find_field_unordered("name1"); name1.error() == SUCCESS) {
         obj.fixed_name_object.name1 = std::string_view(name1);
      }
      if (auto name2 = fixed_name_object.find_field_unordered("name2"); name2.error() == SUCCESS) {
         obj.fixed_name_object.name2 = std::string_view(name2);
      }
      if (auto name3 = fixed_name_object.find_field_unordered("name3"); name3.error() == SUCCESS) {
         obj.fixed_name_object.name3 = std::string_view(name3);
      }
      if (auto name4 = fixed_name_object.find_field_unordered("name4"); name4.error() == SUCCESS) {
         obj.fixed_name_object.name4 = std::string_view(name4);
      }
   }
   
   if (auto another_object = doc.find_field_unordered("another_object"); another_object.error() == SUCCESS) {
      if (auto string = another_object.find_field_unordered("string"); string.error() == SUCCESS) {
         obj.another_object.string = std::string_view(string);
      }
      if (auto another_string = another_object.find_field_unordered("another_string"); another_string.error() == SUCCESS) {
         obj.another_object.another_string = std::string_view(another_string);
      }
      if (auto another_string = another_object.find_field_unordered("escaped_text"); another_string.error() == SUCCESS) {
         std::string_view new_string{};
         std::ignore = another_string.get_string().get(new_string);
         obj.another_object.escaped_text = new_string;
      }
      if (auto boolean = another_object.find_field_unordered("boolean"); boolean.error() == SUCCESS) {
         obj.another_object.boolean = bool(boolean);
      }
      
      if (auto nested_object = another_object.find_field_unordered("nested_object"); nested_object.error() == SUCCESS) {
         if (auto v3s = nested_object.find_field_unordered("v3s"); v3s.error() == SUCCESS) {
            obj.another_object.nested_object.v3s.clear();
            for (ondemand::array v3 : v3s) {
               size_t i = 0;
               auto& back = obj.another_object.nested_object.v3s.emplace_back();
               for (double x : v3) {
                  back[i++] = x;
               }
            }
         }
         
         if (auto id = nested_object.find_field_unordered("id"); id.error() == SUCCESS) {
            obj.another_object.nested_object.id = std::string_view(id);
         }
      }
   }
   
   if (auto string_array = doc.find_field_unordered("string_array"); string_array.error() == SUCCESS) {
      obj.string_array.clear();
      for (std::string_view x : string_array) { obj.string_array.emplace_back(x); }
   }
   
   if (auto string = doc.find_field_unordered("string"); string.error() == SUCCESS) {
      obj.string = std::string_view(string);
   }
   if (auto number = doc.find_field_unordered("number"); number.error() == SUCCESS) {
      obj.number = double(number);
   }
   if (auto boolean = doc.find_field_unordered("boolean"); boolean.error() == SUCCESS) {
      obj.boolean = bool(boolean);
   }
   if (auto another_bool = doc.find_field_unordered("another_bool"); another_bool.error() == SUCCESS) {
      obj.another_bool = bool(another_bool);
   }
   
  return false;
}

auto simdjson_test()
{
   std::string buffer{ json_minified };
   std::string minified = buffer;
   
   size_t new_length{}; // It will receive the minified length.
   [[maybe_unused]] auto error = simdjson::minify(buffer.data(), buffer.size(), minified.data(), new_length);
   minified.resize(new_length);
   
   simdjson::padded_string padded = minified;
   
   on_demand parser{};
   
   obj_t obj{};
   
   auto t0 = std::chrono::steady_clock::now();
   
   try {
      for (size_t i = 0; i < iterations; ++i) {
         const auto error = parser.read_in_order(obj, padded);
         if (error) {
           std::cerr << "simdjson error" << std::endl;
         }
      }
   } catch (const std::exception& e) {
      std::cout << "simdjson exception error: " << e.what() << '\n';
   }
   
   auto t1 = std::chrono::steady_clock::now();
   
   results r{ "simdjson (on demand)", "https://github.com/simdjson/simdjson", iterations };
   
   r.json_byte_length = padded.size();
   r.json_read = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;
   
   r.print();
   
   return r;
}

struct on_demand_abc {
   bool read(abc_t<false>& obj, const simdjson::padded_string &json);
private:
   simdjson::ondemand::parser parser{};
};

#define SIMD_PULL(x) simdjson::ondemand::array x = doc[#x]; obj.x.clear(); for (int64_t value : x) { obj.x.emplace_back(value); }

bool on_demand_abc::read(abc_t<false>& obj, const simdjson::padded_string &json) {
  auto doc = parser.iterate(json);
   
   SIMD_PULL(a); SIMD_PULL(b); SIMD_PULL(c);
   SIMD_PULL(d); SIMD_PULL(e); SIMD_PULL(f);
   SIMD_PULL(g); SIMD_PULL(h); SIMD_PULL(i);
   SIMD_PULL(j); SIMD_PULL(k); SIMD_PULL(l);
   SIMD_PULL(m); SIMD_PULL(n); SIMD_PULL(o);
   SIMD_PULL(p); SIMD_PULL(q); SIMD_PULL(r);
   SIMD_PULL(s); SIMD_PULL(t); SIMD_PULL(u);
   SIMD_PULL(v); SIMD_PULL(w); SIMD_PULL(x);
   SIMD_PULL(y); SIMD_PULL(z);
   
  return false;
}

auto simdjson_abc_test()
{
   std::string buffer = glz::write_json(abc_t<true>{}).value();
   std::string minified = buffer;
   
   abc_t<false> obj{};
   
   size_t new_length{}; // It will receive the minified length.
   [[maybe_unused]] auto error = simdjson::minify(buffer.data(), buffer.size(), minified.data(), new_length);
   minified.resize(new_length);
   
   simdjson::padded_string padded = minified;
   
   on_demand_abc parser{};
   
   auto t0 = std::chrono::steady_clock::now();
   
   try {
      for (size_t i = 0; i < iterations_abc; ++i) {
         const auto error = parser.read(obj, padded);
         if (error) {
           std::cerr << "simdjson error" << std::endl;
         }
      }
   } catch (const std::exception& e) {
      std::cout << "simdjson exception error: " << e.what() << '\n';
   }
   
   auto t1 = std::chrono::steady_clock::now();
   
   results r{ "simdjson (on demand)", "https://github.com/simdjson/simdjson", iterations_abc };
   
   r.json_byte_length = padded.size();
   r.json_read = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;
   
   r.print(false);
   
   return r;
}

static constexpr std::string_view table_header = R"(
| Library                                                      | Roundtrip Time (s) | Write (MB/s) | Read (MB/s) |
| ------------------------------------------------------------ | ------------------ | ------------ | ----------- |)";

static constexpr std::string_view table_header_read = R"(
| Library                                                      | Read (MB/s) |
| ------------------------------------------------------------ | ----------- |)";

void test0()
{
   std::vector<results> results;
   results.emplace_back(glaze_test<glz::opts{}>());
   results.emplace_back(jsonifier_test());
   results.emplace_back(simdjson_test());
   
   std::ofstream table{ "json_minfied_stats.md" };
   if (table) {
      const auto n = results.size();
      table << table_header << '\n';
      for (size_t i = 0; i < n; ++i) {
         table << results[i].json_stats();
         if (i != n - 1) {
            table << '\n';
         }
      }
   }
}

void abc_test()
{
   std::vector<results> results;
   results.emplace_back(glaze_abc_test());
   results.emplace_back(jsonifier_abc_test());
   results.emplace_back(simdjson_abc_test());
   
   std::ofstream table{ "json_stats_abc.md" };
   if (table) {
      const auto n = results.size();
      table << table_header_read << '\n';
      for (size_t i = 0; i < n; ++i) {
         table << results[i].json_stats_read(false);
         if (i != n - 1) {
            table << '\n';
         }
      }
   }
}

int main()
{
   test0();
   abc_test();
   
   return 0;
}
