> [!NOTE]
> Some of the features used in this README might not work on GitHub.  
> For the best reading experience, please consider viewing it in Visual Studio Code with suitable extensions.  
> Thank you for your understanding.

<div style="position: absolute;">

<div style="position: relative; display: flex; align-items: center; gap: 15px;">
  <img
    src="LAUNCH.svg"
    alt="Logo of LAUNCH"
    style="height: 75px;"
  />
  <h1>LAUNCH - Introduction</h1>
</div>

<h2 style="
  position: relative;
  top: -70px;
  left: 330px;
  color: orange;
  transform-origin: 20%;
  animation: splash 1.2s ease-in-out infinite;
">Now with splashes!</h2>

</div>

<div style="height: 100px;"></div>

---

![](https://img.shields.io/badge/Language-C++-blue) ![](https://img.shields.io/badge/Standard-C++20-blue) ![](https://img.shields.io/badge/Dependencies-STL_Only-green) ![](https://img.shields.io/badge/License-MIT-yellow) ![](https://img.shields.io/badge/Platform-Any-lightgrey)

## Navigation

[Part I: FAQs](#part1)  
[Part II: Building / Using](#part2)  
[Part III: The Modules](#part3)  
[Part IV: Conclusion / TL;DR](#part4)

<h2 id="part1">Part I: FAQs</h2>

| Qs | As |
|-|-|
| Where does the name *LAUNCH* come from? | It's a backronym - **L**ibrary of **A**bundant **U**tilities for **N**ew **C**reative **H**acks! |
| How many modules are there? | Currently, there are 8 (LAUNCH itself) + 3 (LEISURE features). |
| Well, what's *LEISURE*, then? | It's still a backronym - **L**ibrary of **E**xperimental **I**deas for **SURE**! |
| Hmm, I wonder what we're doing here. | Okay, it's called Socratic questioning. |
| Fine! | Then let's move on! |

<h2 id="part2">Part II: Building / Using</h2>

### Part II-1: Building

This project uses CMake.  
For details, please refer to `CMakeLists.txt`.

### Part II-2: Using

Simply write `#include "include/launch.h"`.  
If you need to control which modules will be included, please `#define` corresponding macros in `include/launch_config.h`.  
The macros look like this:
```cpp
// disabling a specific module:
#define LAUNCH_NO_<MODULENAME>
// enabling LEISURE features:
#define LAUNCH_EXPERIMENTAL
// simplifying namespaces:
#define LAUNCH_SIMPLIFY_NAMESPACE
```
Code snippets in this README work well with the macros below:
```cpp
#define LAUNCH_EXPERIMENTAL
#define LAUNCH_SIMPLIFY_NAMESPACE
```

<h2 id="part3">Part III: The Modules</h2>

### \<chroyes\>

- What does the name mean?  
**-** \<chro**no**\> versus \<chro**yes**\>
- What can \<chroyes\> do?
```cpp
l::stopwatch watch;
watch.start();
/* do something */
std::this_thread::sleep_for(1s);
watch.pause();
/* take a rest */
std::this_thread::sleep_for(1s);
watch.resume();
/* back to work */
std::this_thread::sleep_for(1s);
watch.stop();
std::cout << "Time elapsed: " << watch.get_dur().milliseconds() << " ms\n";
```
- Expected output:
```
Time elapsed: 2000 ms
```

### \<clap\>

- What does the name mean?  
**-** **C**ommand **L**ine **A**rgument **P**arser
- What can \<clap\> do?
```cpp
int main(int argc, char* argv[]) {
    l::clap parser(argc, argv, {{"o", "option"}, {"f", "flag"}});
    // the third parameter (aliases) maps short options to corresponding long options
    // aliases is an empty std::unordered_map by default
    std::optional<std::string> option = parser.get_opt("option");
    bool flag = parser.get_flag("flag");
    // clap only recognizes long options
    if (option) {
        std::cout << option.value() << "\n";
    }
    std::cout << std::boolalpha << flag << "\n";
    return 0;
}
```
- Expected output (with no parameters):
```
false
```
- Expected output (with parameter `--flag` or `-f`):
```
true
```
- Expected output (with parameter `--option <value>` or `-o <value>`):
```
<value> false
```
- Expected output (with parameter `--flag --option <value>` or `--flag -o <value>` or `-f --option <value>` or `-f -o <value>` or `-fo <value>` or `--option <value> --flag` or `--option <value> -f` or `-o <value> --flag` or `-o <value> -f`):
```
<value> true
```

### \<escseq\>

- What does the name mean?  
**-** ANSI **Esc**ape **Seq**uences (ANSI Escape Code)
- What can \<escseq\> do?
```cpp
std::cout << l::gen_style(l::escseq_style::italic | l::escseq_style::underline)
          << l::foreground_color(0, 0, 255)
          << l::background_color(255, 255, 255)
          << "Hyperlink"
          << l::rendl_fast;
```
- Expected output: <p style="font-style: italic; text-decoration: underline; color: #00f; background-color: #fff;">Hyperlink</p>

### \<fmtio\> & \<hedgehog\>

- *(\<fmtio\>)* What does the name mean?  
**-** **F**or**m**a**t**ted **I**nput/**O**utput
- *(\<hedgehog\>)* What does the name mean?  
**-** You see, hedgehogs' spines don't care what they're carrying - that's what `hedgehog` does!
- What can they do?
```cpp
l::fmtout("int: {}, double: {}, bool: {}, string: {}, char: {}\n", { 42, 3.14, true, "Hello World!", 'A' });
int a;
double b;
l::fmtin("a: {}, b: {}\n", l::pack(a, b));
l::fmtout("You entered: {} & {}.\n", { a, b });
```
- Expected output:
```
int: 42, double: 3.14, bool: true, string: Hello World!, char: A
a: <a>, b: <b>
You entered: <a> & <b>.
```

### \<goodmath\>, \<goodrand\> & \<goodstr\>

- What do the names mean?  
**-** These names distinguish LAUNCH modules from STL ones.
- What can they do?
```cpp
std::string hint = "Sine value of ";
l::randgen_real rand(0.0, 360.0);
double deg = rand.next();
std::cout << hint << deg << "deg: " << l::sin(l::dtor(deg)) << "\n";
std::cout << l::replace(hint, "Sine", "Cosine");
std::cout << hint << deg << "deg: " << l::cos(l::dtor(deg)) << "\n";
```
- Expected output:
```
Sine value of <random>deg: <sin(random)>
Cosine value of <random>deg: <cos(random)>
```

### \<arithing\> (LEISURE feature)

- What does the name mean?  
**-** **Ari***th*metic *Th***ing**s
- What can \<arithing\> do?
```cpp
std::cout <<
    "Very positive: " <<
    l::arint(std::numeric_limits<unsigned long long>::max()) <<
    "\n";
std::cout <<
    "Very negative (hey, it's way too negative): " <<
    l::arint(std::numeric_limits<unsigned long long>::max(), l::arint_specval::neg) <<
    "\n";
std::cout <<
    "Wait, what?: " <<
    l::arint(0, l::arint_specval::inf) <<
    " " <<
    l::arint(0, l::arint_specval::inf | l::arint_specval::neg) <<
    " " <<
    l::arint(0, l::arint_specval::nan) <<
    "\n";
std::cout << "Ooh, rational: " << l::arrat(22, 7) << "\n";
```
Expected output:
```
Very positive: 18446744073709551615
Very negative (hey, it's way too negative): -18446744073709551615
Wait, what?: inf -inf NaN
Ooh, rational: 22/7
```

### \<exfmtio\> (LEISURE feature)

- What does the name mean?  
**-** **Ex**tended **F**or**m**a**t**ted **I**nput/**O**utput
- What can \<exfmtio\> do?
```cpp
l::nullout << "Hello World...? Anyone here?\n"; // nullout is a global nullstream instance
l::hedgehog hh;
l::fmtout("Enter anything: ");
l::fmtin_deduce_auto(hh);
hh.push_back(hh[0].type().name());
l::fmtout("Got {0} with type {1}\n", hh);
```
Expected output:
```
<nothing>
Enter anything: <something>
Got <something> with type <type name of something>
```

### \<genev\> (LEISURE feature)

- What does the name mean?  
**-** **Gen**erally **Ev**erything
- What can \<genev\> do?
```cpp
l::genev mass = 1 * kilo(gram), acceleration = 9.8 * meter / (second ^ 2);
std::cout << "F = ma = " << mass << " * " << acceleration << " = " << mass * acceleration << "\n";
```
Expected output:
```
F = ma = 1 kg * 9.8 m/s^2 = 9.8 m*kg/s^2
```

<h2 id="part4">Part IV: Conclusion / TL;DR</h2>

In general, LAUNCH is a collection of utilities that solves everyday problems easily and quickly.  
Why not use LAUNCH?

---

<small>Author: QuectoRime Studios<br>License (of this document): CC BY-SA 4.0</small>

<style>
@keyframes splash {
  0% {
    transform: rotate(-10deg) scale(1);
  }
  50% {
    transform: rotate(-10deg) scale(1.1);
  }
  100% {
    transform: rotate(-10deg) scale(1);
  }
}
</style>