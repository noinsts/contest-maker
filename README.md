# 📦 Contest Maker

**Contest Maker** - це декстопна GTK-програма, яка допомагає швидко створити структуру контесту
Вона автоматизує створення директорій і вихідних файлів для задач. Додає базові шаблони

## UI прев'ю

### Windows

<img src="assets/windows.png" width="200" alt="Тут повинна бути картинка">

### Linux

<img src="assets/linux.png" width="400" alt="Тут повинна бути картинка">

## Templates

### C++

```cpp
#include <bits/stdc++.h>
using namespace std;

void solve() {
    // TODO: code
}

int main() {
    ios_base::sync_with_stdio(false):
    cin.tie(nullptr);

    int tt; cin >> tt;

    while (tt--) {
        solve();
    }

    return 0;
}
```

### Java

```java
import java.io.*;

public class Name {
    private static void solve() {
        // TODO: code
    }

    public static void main(String[] args) throws IOException {
        try (BufferedReader sc = new BufferedReader(new InputStreamReader(System.in))) {
            int tt = Integer.parseInt(sc.readLine());

            while (tt-- > 0) {
                solve();
            }
        }
    }
}
```

## Install

### Linux

1. **Встанови залежності**
	
	На Ubuntu/Debian:

	```bash
	sudo apt update
	sudo apt install build-essential cmake ninja-build pkg-config libgtkmm-3.0-dev
	```

	На Fedora:

	```bash
	sudo dnf install cmake ninja-build gcc-c++ gtkmm30-devel
	```

	На Arch:

	```bash
	sudo pacman -S base-devel cmake ninja gtkmm3
	```

2. **Запусти скрипт збірки** з кореня проекту:

	```bash
	bash build.bash
	```

### Windows

1. **Встановлюємо [MSYS2](https://www.msys2.org/)**
2. **Оновлюємо пакети**

	```bash
	pacman -Syu
	```

3. **Встановлюємо інструменти та GTK:**

	```bash
	pacman -S mingw-w64-x86_64-toolchain \
          mingw-w64-x86_64-gtkmm3 \
          mingw-w64-x86_64-cmake \
          mingw-w64-x86_64-pkg-config
	```

4. **Запускаємо білд**

	```bash
	bash build.bash
	```


## 🛡 Ліцензія  
Цей проєкт ліцензовано під [MIT License](./LICENSE).

## ✨ Автор
**[@noinsts](https://github.com/noinsts)** – автор і розробник цього репозиторію.
