#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "vector.h"
#include <iostream>

using std::cout;
using std::endl;

int cnt_constructor = 0;
int cnt_destructor = 0;


struct Car {
    Car(int n = 0):num(n){
        cnt_constructor++;
    }
    ~Car() {
        cnt_destructor++;
    }
    int num;
};

struct Dog {
    Dog(int n = 0):num(n){
        cnt++;
    }
    Dog(const Dog& other) = default;
    Dog(Dog&& other) = default;
    Dog& operator= (const Dog& other) = default;
    Dog& operator= (Dog&& other) = default;
    ~Dog() {
        cnt--;
    }
    int num;
    static int cnt;
};

int Dog::cnt = 0;

TEST_CASE("vectors can be sized and resized", "[vector]") {
    // Default constructor
    clstd::vector<int> v(5);
    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);

    clstd::vector<int> v_copy(v);
    REQUIRE(v.size() == v_copy.size());

    clstd::vector<int> v_assign(3);
    v_assign = v;
    REQUIRE(v.size() == v_assign.size());

    clstd::vector<int> v2(5, 871);
    REQUIRE(v2[0] == 871);
    REQUIRE(*(v2.data() + 1) == 871);
    int sum = 0;
    for(auto it = v2.begin(); it != v2.end(); ++it) {
        sum += *it;
    }
    REQUIRE(sum == 4355);
    REQUIRE(v2.front() == 871);
    REQUIRE(v2.back() == 871);

    //clstd::vector<Car> vcar(3, Car(466));
    clstd::vector<Car> vcar(3, 466);
    REQUIRE(vcar[1].num == 466);

    /*
    SECTION( "resizing bigger changes size and capacity" ) {
        v.resize( 10 );
        
        REQUIRE( v.size() == 10 );
        REQUIRE( v.capacity() >= 10 );
    }
    */
}

TEST_CASE("move semantics", "[vector]") {
    {
        clstd::vector<Dog> v = clstd::vector<Dog> {1, 2, 3, 4, 5};
        REQUIRE(Dog::cnt == 5);
    }
    REQUIRE(Dog::cnt == 0);
}

TEST_CASE("push_back", "[vector]") {
    clstd::vector<Car> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);
    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);
    REQUIRE(v[0].num == 1);
    REQUIRE(v[1].num == 2);
    REQUIRE(v[2].num == 3);
    REQUIRE(v[3].num == 4);
    REQUIRE(v[4].num == 5);

    v.shrink_to_fit();
    REQUIRE(v.size() == v.capacity());
    REQUIRE(v.capacity() == 5);
}



TEST_CASE("memory leak", "[vector]") {
    REQUIRE(cnt_constructor == cnt_destructor);
}
