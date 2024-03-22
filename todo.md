# lesson summaries

## 3/15

- over the past two Lessons, we:
  - reviewed/discussed UTs - reuse, layered - *e.g.*, `encode()​`'s UTs using `encode_char()`'s​ UTs
  - although we couldn't remember the precise **need**, we discussed variadic f()​-s (`<stdarg.h>`...`va_start` and friends and macros​ (`__VA_ARGS__`, `__VA_OPT__​`):
    - [Replacing text macros - cppreference.com](https://en.cppreference.com/w/cpp/preprocessor/replace)
    - [vprintf, vfprintf, vsprintf, vsnprintf, vprintf_s, vfprintf_s, vsprintf_s, vsnprintf_s - cppreference.com](https://en.cppreference.com/w/c/io/vfprintf)
  - try and understand (e.g., in a debugger...single-step...) the example code from the latter:

    ```C
    #include <stdio.h>
    #include <stdarg.h>
    #include <time.h>
    
    void debug_log(const char *fmt, ...)
    {
        struct timespec ts;
        timespec_get(&ts, TIME_UTC);
        char time_buf[100];
        size_t rc = strftime(time_buf, sizeof time_buf, "%D %T", gmtime(&ts.tv_sec));
        snprintf(time_buf + rc, sizeof time_buf - rc, ".%06ld UTC", ts.tv_nsec / 1000);
    
        va_list args1;
        va_start(args1, fmt);
        va_list args2;
        va_copy(args2, args1);
        char buf[1+vsnprintf(NULL, 0, fmt, args1)];
        va_end(args1);
        vsnprintf(buf, sizeof buf, fmt, args2);
        va_end(args2);
    
        printf("%s [debug]: %s\n", time_buf, buf);
    }
    
    int main(void)
    {
        debug_log("Logging, %d, %d, %d", 1, 2, 3);
    }
    ```

  - and try writing your own variadic `f()​`, maybe for error messages
  - reviewed otp​'s current behavior, which was...confusing 😎...given the hacked command line and default values in the code
  - further, the approach you chose - based on indices and not values - is original and *may* be better (*i.e.*, order of  symbols in *alphabet*) than the p-code we wrote:

  ```C
      ciphertext_char -= 'A';
      char plain_char = ciphertext_char - key_char; //
    
      if (plain_char < 0) {
          plain_char += alpha_length;
      }
    
      return plain_char % alpha_length;
  ```

  - reviewed the examples in the PDF using a *row* (*e.g.*, `encode()` & *column* (*e.g.*, `encode_char()`) approach

### next - 3/15

- prove, through explanation and UTs, that your index-based method is better than above - as I contend above is simpler
- let's discuss standard handles and command line redirection again, as **I think** it's simpler than **you think** 😎

## 3/1

- discussed the concept of *parameterization* and how it is **exteremely** important to *reusability*, *testability*, *etc.*
- reviewed the `assert()` *macro* and how it accepts any `boolean` expression
  - this led to a discussion on *style* and how I try to represent *ranges* similiar to how they would be represented mathematically:  
      `low <= value && value < high`  
      compared to  
      $low \le value \lt high$
- concluded w/a discussion on *pure* `f()`-s - and how they don't reference *anything* outside of their *scope* - **no side-effects** (*e.g.*, global vars), *including* `I/O`

  > If a program doesn't modify its output, there will be no evidence that it ran.  
  > Clifford Schaeffer (I think😎)

### next - 3/1

- UTs for `encode()` & `encode_char()`
- review references in prep for writing `variadic` *functions* and *macros*

### references - 3/1

- [assert - cppreference.com](https://en.cppreference.com/w/c/error/assert)
- [Variadic functions - cppreference.com](https://en.cppreference.com/w/c/variadic)
- [Replacing text macros - cppreference.com](https://en.cppreference.com/w/c/preprocessor/replace)
- [C Language Tutorial => Variable arguments](https://riptutorial.com/c/topic/455/variable-arguments)
- [C Language Tutorial => Variadic arguments macro](https://riptutorial.com/c/example/18583/variadic-arguments-macro)

## 2/25

- restructured code to expose `test_suite` amid a discussion on [storage-class specifiers](https://en.cppreference.com/w/c/language/storage_duration) - `extern` and `static`
- discussed random numbers ([PRNG - pseudo-random number generation](https://en.cppreference.com/w/c/numeric/random)), and how in `otp`, the `seed` and length of the `alphabet` are *directly* related to the sequence of numbers returned from `rand()`
- pair-programmed the p-code for `encode()`
  - discussed where the I/O and loop should exist
  - discussed how to iterate over the `key` and `plain_text` - use a single index
  - how the modulus (`%`) operator can be used to keep an integer (*e.g.*, an array index) w/in a range

### next - 2/25

- oodles and *oodles* of UTs

## 2/18

- tweaked `Makefile` to suppress compiler warnings
- ran some experiments (e.g., `sleep()`​) to understand why `get_random_number()​​` was returning the same number
  - clock frequency
- leveraged the existing `main()`​ to generate an array of expected​ (AKA *golden*, *base*, *known*) random numbers based on a known seed
  - use alternate tools to generate known values
- UT led us to understand the relationship between `srand()`​'s `seed​`, and the length of the `alphabet​`:
  - `get_random_number()`​​ likely needs to be parameterized over the length of the `alphabet`

### next - 2/18

- oodles of UTs

## 2/9

- discussed priorities/progress and being in the right *mood* for differrent *types* of work and taking advantage of *time-shifting*
- reviewed how we partitioned `otp` into two parts:
  - **UI**: command line args
    - testing via invocations w/combinations of CLAs, driven from a script (black-box)
  - **algorithm**: key generation, encoding and decoding related `f()`-s
    - testing via μunit tests (white-box)
- wrote a simple UT - `encode_one_char()`  - using the *AAA* pattern

### next - 2/9

- using μunit's `setup()` and `teardown()` (if needed) to setup known (*expected*) `key`, `plain text` and `cipher text` values for use in UTs
- write both *positive* (**S**ubject **U**nder **T**est - in this case the `f()` under test - passes) and *negative* (SUT fails...but UT still passes as failure is the *expected* value) UTs

- **Arrange, Act and Assert Pattern**: [The Three A's of Unit Testing | Rob Marshall](https://robertmarshall.dev/blog/arrange-act-and-assert-pattern-the-three-as-of-unit-testing/)
- **Unit Testing — AAA pattern.**[Unit testing is one of the Extreme… | by Supreet Singh | Xebia Engineering Blog | Medium](https://medium.com/xebia-engineering/unit-testing-aaa-pattern-ab1c08737d53)

## 12/31

- discussed holiday celebrations
- had a lengthy discussion on how you appreciated the delination (client/server, UI/functionality...) and how the principal of *separation of concerns* (SoC) is a good thing - based on *input UI* (in `otp` - the command line), *transformation* (computation) and *output UI*
- added *help* CLA but introduced a bug along the way

### next - 12/31

>> same

---

## 12/24

- discussed `FILE*` vs. file descriptors (`int`) - see [stdio](https://www.man7.org/linux/man-pages/man3/stdio.3.html) versus [fcntl](https://www.man7.org/linux/man-pages/man2/fcntl.2.html)
  - [fileno](https://www.man7.org/linux/man-pages/man3/fileno.3.html) extracts the`fd` from a given `FILE*`
- had a discussion on the approach you were taking - implementing functionality - and the approach I wanted you to take, which is to get the UI (CLAs) working first, using more of a *top-down* approach and *stubs*, and then use TDD to implement the actual functionality (vs. the initial *stubs*)

>>> - I think we're on the same page now - **yes?**

- talked through the *pure* and *adulterated* approaches to TDD
  - essentially when the `.h` file is created for the *SUT* (subject under test), *e.g.*, `encode()`, `decode()`, *etc.* - due to modern-day tooling

### next - 12/24

- [ ] complete the UI
  - [ ] implementation
  - [ ] testing (script of different valid/invalid invocations)

---

## 12/16

- couldn't find a definition for *decoupling*, but I think *parameterization* is a better term:
  - adds flexibility - including testability
  - makes for more concise code
  - make code easier to maintain
- a summary of [Linux Signals](<misc/Linux Signals.md>)
- a summary of `C` [compiler phases](https://en.cppreference.com/w/c/language/translation_phases)
  > - let's look at the `-E` option again
- patched read loop, to make forward progress, but introduced a constraint WRT `\n`:
  - need to do some research on how different *read* functions handle `\n` and/or when `stdin` is the keyboard

## next 12/16

- [ ] keep making forward progress on completing the UI
  > you can always go back and fix issues in a second pass
- [ ] create a UI test script w/different invocations
  - [ ] consider creating *golden* test files and automate further by comparing ([diff(1): compare files line by line](https://linux.die.net/man/1/diff)) **actual** to **expected** (golden)

---

## 12/12

- bug fixing...**AKA** how to pass command line argument(s) (CLAs) to `otp` when using the debugger
- started to evolve *stubs* (signatures, mock return values) to their final version
  - pair-programmed (PP) `keygen()` *stub*
  - discussed *p-code* for `encode()`/`decode()`

### next - 12/12

- [ ] see: **next - 12/10** 😎
- [ ] more specifically;  `encode()`/`decode()` to level of `key_gen` (*stub*) so UI can be completed - implementation, test and structure/documentation

### references - 12/12

## 12/10

- *stubs* - in this/our context - are merely function definitions (*i.e*., implementations) that allow you to make progress, *e.g.*,
  > **reminder** - if there's a reference to a (function) - a *declaration* enables compilation, a *defintion* enables linking

  ```C
  return_type encode(...params...) {
    fprintf(stderr, "encode: ...\n", params);
    return success;
  }
  ```

- enabling compilation (declarations) and linking (definitions) are steps in the **TDD** process
- **that's all folks**...Lesson displaceed by tickets to:
- ![Go Hawks](https://th.bing.com/th?id=OIF.ypvcLGQKBXGC%2f2ca%2bYR5qg&rs=1&pid=ImgDetMain)
  - **may the best team win**...*kidding*...**Go Hawks**❗❗❗

### next - 12/10

- see: **next - 12/06**

## 12/06

- discussed different ways of looking at life and how we feel both/either at different points in time😎
![Pushing Boulder Uphill](https://miro.medium.com/max/880/1*dKf6Q2WJ1RGZlPjvUbPNFw.gif "Pushing Boulder Uphill")

![Biorhythms](https://www.intmath.com/trigonometric-graphs/img/biorhythms.gif "Biorhythms")

> not that I *believe* in biorhythms; but their *wave chart* works well for illustrative purposes😎

- fixed string comparison w/macro
- structured `main()` to execute each command
  - refactored for common file I/O in `encode()` and `decode()`
- tweaked makefile, fixed syntax errors...**builds successfully**❗

### next - 12/06

- [ ] create list of UTs for testing the UI (command line)
  - [ ] pass all of these (manual) UTs
- [ ] stubs for `key()`/`encode()`/`decode()`
- [ ] we need to review tdd/aaa & µnit

---

## 11/25

- caught-up on life, and realized that we both have a lot to be thankful for...
- complimentary discussions on:
  - new, exciting work opportunities
  - objectives (personal/pofessional) and *projects*
- yielded:
  - **backburnered** *projects:*  
    ~~1. **job/career**~~  
      ~~- LinkedIn~~  
        ~~- [ ] review other’s LI style to determine what’s next~~  
      ~~- resume~~  
        ~~- [ ] resume *v.Next*~~  
    ~~2. **LI projects**~~  
      ~~- [ ] review, build, *etc.*~~  
  - **focussed** *projects:*  
    - **otp**
        1. [ ] user interface:  
          - design and implement (`getopt()` loop) command line args, input/output files (named and standard), *etc.*  
        2. [ ] review (preferably in debugger) µnit's `example.c`

---

## 11/04

- discussion on the state of the world... 🎼... Be happy❗😎
- *projects:*
  1. **job/career**
     - LinkedIn
       - [ ] review other’s LI style to determine what’s next
     - resume
       - [ ] resume *v.Next*
  2. **LI projects**
     - [ ] review, build, *etc.*
  3. **otp**
      - [ ] command line args, std files and named files – convention for *standard* handles is `-`
      - [ ] spec out/implement CLA/getopt(0 loop for otp…

---

## 10/28

- let's organize around three *projects*:
  1. **job/career**
     - LinkedIn
       - [ ] review other’s LI style to determine what’s next
     - resume
       - [ ] resume *v.Next*
  2. **LI projects**
     - [ ] review, build, *etc.*
  3. **`otp`**:
     - Still organizing – **gitpod**, **kanbn**, **todo tree**, **live share**…
       - [ ] take a look at Kanbn and to do tree
         - [ ] [Kanbn Extension for Visual Studio Code - Visual Studio Marketplace](https://marketplace.visualstudio.com/items?itemName=gordonlarrigan.vscode-kanbn)
         - [ ] [Todo Tree - Visual Studio Marketplace](https://marketplace.visualstudio.com/items?itemName=Gruntfuggly.todo-tree)
     - [ ] make a makefile
       - create your own as the `replit.com` version won't work w/multiple `main()`-s - `otp` and `munit`
       - ~~potentially use repl's makefile~~
         - ~~tell it to show hidden file (hides makefile)~~
     - [ ] review [µnit](https://nemequ.github.io/munit/) docs
       - [ ] download `.c` and `.h` file and upload to `otp` repo
       - [ ] step through `munit` example code to refresh memory
     - [ ] design all three commands for commandline
       - refer back to notes from Wyzant notes (add to repo)

---

## 10/20

- feeling better about job vs. career
- discussed LinkedIn (LI) style (too informal?) and migrating your resume’s *About Me* section to an *Objectives* section
- decided to stick with – and I won’t tempt you with other projects 😎 – `OTP` as it’s already a known quantity, and is written to be net-centric (client/server – C/S)  
  - as an aside, future projects – like `Huffman` – can be architected to be C/S as well
- did a deep-ish dive into :
  - [getopt(3): Parse options - Linux man page](https://linux.die.net/man/3/getopt)

### next - 10/20

- [ ] complete dev. env. setup
  - github/replit/gitpod
- VS Code, consider these extensions (they don't work in any of the browser envs.):
  - [Kanbn Extension for Visual Studio Code - Visual Studio Marketplace](https://marketplace.visualstudio.com/items?itemName=gordonlarrigan.vscode-kanbn)
- [ ] review other’s LI style to determine what’s next
- [ ] resume *v.Next*
- [ ] recreate `OTP` repl (missing `makefile`) as it’s early and there’s very little to migrate
  - When complete, push all updates to repo
- [ ] design all three command lines
- [ ] implement `keygen`

---

## 10/13

- complete dev. env. setup
  - github/replit/gitpod
  - VS Code, consider these extensions (they don't work in any of the browser envs.)
    - [Live Share - Visual Studio Marketplace](https://marketplace.visualstudio.com/items?itemName=MS-vsliveshare.vsliveshare)
      - [Use Microsoft Live Share to collaborate with Visual Studio Code](https://code.visualstudio.com/learn/collaboration/live-share)
      - [Install and sign in to Live Share in Visual Studio Code - Live Share | Microsoft Learn](https://learn.microsoft.com/en-us/visualstudio/liveshare/use/install-live-share-visual-studio-code)
    - [Kanbn Extension for Visual Studio Code - Visual Studio Marketplace](https://marketplace.visualstudio.com/items?itemName=gordonlarrigan.vscode-kanbn)
- create a skeleton `main()` that can choose to then run - `keygen`, `encrypt`, `decrypt` - based on a command line option
  - consider: [getopt(3) - Linux manual page](https://www.man7.org/linux/man-pages/man3/getopt.3.html)
