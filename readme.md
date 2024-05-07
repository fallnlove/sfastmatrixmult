# SFastMatrixMultiplication Lib

Библиотека предназначена для быстрого матричного умножения. Подключается через CMake.

## Реализованные функции умножения

### Наивное умножение

Если ваш код стремится экономить память и вам не сильно критично
время исполнения кода, то можете использовать наивное умножение.

```cpp
#include<s_fast/s_fast.h>

using namespace s_fast;

int main() {
    Matrix<int> a({{1, 1},
                   {0, 2}});
    Matrix<int> b({{1, 0},
                   {1, 2}});
    std::cout << SimpleMultiplication(a, b) << std::endl;
    // 2 2
    // 2 4
}
```

### Наивное умножение с транспонированием

Данная функция требует $O(nm)$ дополнительной памяти
для аллокации новой матрицы. Если у вас на процессоре
доступны векторные инструкции, то не используйте
эту функцию, используйте вместо нее функцию с
simd инструкциями.

```cpp
#include<s_fast/s_fast.h>

using namespace s_fast;

int main() {
    Matrix<int> a({{1, 1},
                   {0, 2}});
    Matrix<int> b({{1, 0},
                   {1, 2}});
    std::cout << SimpleMultiplicationWithTranspose(a, b) << std::endl;
    // 2 2
    // 2 4
}
```

### Умножение с simd инструкциями

Данная функция также требует $O(nm)$ дополнительной памяти
для аллокации новой матрицы. Функция использует
векторные инструкции, на этапе компиляции она сама подбирает
максимальный доступный уровень инструкций и использует их.

```cpp
#include<s_fast/s_fast.h>

using namespace s_fast;

int main() {
    Matrix<int> a({{1, 1},
                   {0, 2}});
    Matrix<int> b({{1, 0},
                   {1, 2}});
    std::cout << SimdMultiplication(a, b) << std::endl;
    // 2 2
    // 2 4
}
```

### Алгоритм Штрассена

Это самая быстрая функция из всех представленных, она
основа на Алгоритме Штрассена и оптимизирована мною для
увелечения производительности. Использует много дополнительной
памяти.

```cpp
#include<s_fast/s_fast.h>

using namespace s_fast;

int main() {
    Matrix<int> a({{1, 1},
                   {0, 2}});
    Matrix<int> b({{1, 0},
                   {1, 2}});
    std::cout << Strassen(a, b) << std::endl;
    // 2 2
    // 2 4
}
```

### CacheOblivious умножение

Эта функция была реализована для исследования
производительности, не рекомендуется ее использовать
Алгоритм Штрассена в любом случае будет быстрее.


```cpp
#include<s_fast/s_fast.h>

using namespace s_fast;

int main() {
    Matrix<int> a({{1, 1},
                   {0, 2}});
    Matrix<int> b({{1, 0},
                   {1, 2}});
    std::cout << CacheObliviousMult(a, b) << std::endl;
    // 2 2
    // 2 4
}
```

## Короткая инструкция

В библиотеке реализован класс матриц `Matrix`. Он является
шаблонным, поэтому вы можете сделать матрицу от любого типа.
Реализованы все стандартные операции для матриц:

- обращение по индексу
- сумма и разность двух матриц
- умножение матрицы на скаляр
- транспонирование матрицы

Для большего понимание пример кода:

```cpp
#include<s_fast/s_fast.h>

using namespace s_fast;

int main() {
    Matrix<int> a(3, 3);
    Matrix<int> b(3, 3);
    a(0, 1) = -1;
    b(1, 2) = 1;
    a += b;
    std::cout << Transpose(a) << std::endl;
    //  0 0 0
    // -1 0 0
    //  0 1 0
}
```

Так же доступна функция создания случайной матрицы.
На вход она принимает размеры и генератор случайных
чисел.

```cpp
#include<random>
#include<s_fast/s_fast.h>

using namespace s_fast;

int main() {
    Matrix<int> a = Random<int>(2, 3, std::uniform_int_distribution<int>(0, 2));
    std::cout << a << std::endl;
    // 0 2 1
    // 2 0 0
}
```

## Подключение библиотеки

Допустим вы решили использовать библиотеку **SFast** для своего проекта.
Вы можете подключить ее через `git submodules` или через `CMake FetchContent`.
Для примера будем считать, что вы подключили ее через `submodules`. Тогда
в файле `CMakeLists.txt` своего проекта вам надо будет прописать

```cmake
add_subdirectory("sfastmatrixmult")

include(FetchContent)
FetchContent_Declare(
  xsimd
  GIT_REPOSITORY https://github.com/xtensor-stack/xsimd.git
  GIT_TAG 13.0.0
)
FetchContent_MakeAvailable(xsimd)

target_link_libraries(
  PUBLIC
  xsimd
  s_fast::s_fast
)
```

В случае `CMake` все будет аналогично. В первой строке
вы подключаете библиотеку, далее подтягиваете зависимости
для нее.

## Сборка тестов

Если вы хотите собрать тесты или запустить бенчмарки, то
выполните следующие команды.

Для тестов:

```sh
git clone git@github.com:fallnlove/sfastmatrixmult.git
cd sfastmatrixmult
mkdir build
cd build
cmake ..
make test_mult
./test_mult
```

Для бенчмарков:

```sh
git clone git@github.com:fallnlove/sfastmatrixmult.git
cd sfastmatrixmult
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=RELEASE ..
make bench_mult
./bench_mult
```

