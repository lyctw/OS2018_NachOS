# OS2018_NachOS

Repository for Operating System Course Labs

### What Is Nachos?

* Designed by Thomas Anderson at UC Berkeley in 1992
* Written in C++, has JAVA version
    * We use the C++ version here
* Educational OS that some components can be implemented by users
    * <span style="color:red;">Process management</span>
    * <span style="color:red;">CPU scheduling</span>
    * <span style="color:red;">Memory management</span>
    * File system management
    * Networking



---

### What Is Nachos? (Cont.)

* Nachos is just a UNIX process runs on real OS
* Nachos uses simple MIPS R2/3000 instructions
* Turn program to MIPS binary code


---

### Installation of Nachos
Linux only ...



---

### Linux Machine Only

* Theoretically, You can use any Linux distribution as your platform
    * Ubuntu, Fedora, Debian...
    * Remember to use <span style="color: red;">32-bit</span> instead of 64-bit
        * Nachos4.0 said to support 64-bits but not been completely tested

* Recommended choice:
    * 32-bit Ubuntu 14.04 

---

### Environment


![](https://i.imgur.com/Uy1BJ0C.png)


---

### NachOS Software Structure

![](https://i.imgur.com/bslYvc7.png)



---

### Download & Install VirtualBox

* Oracle VM VirtualBox
    * https://www.virtualbox.org/
* Download 32-bit Ubuntu 14.04 
    * http://releases.ubuntu.com/14.04/


---

### Setup 

Install g++ and csh
```shell=
sudo apt-get install g++
sudo apt-get install csh
sudo apt-get install make
```

Download NachOS & Cross Compiler
```shell=
curl -O http://homepage.ntu.edu.tw/~r07943154/files/OS2019/nachos-4.0.tar.gz
curl -O http://homepage.ntu.edu.tw/~r07943154/files/OS2019/mips-decstation.linux-xgcc.gz
```

---

### Install NachOS 

untar **nachos-4.0.tar.gz**

```shell=
tar -xvf nachos-4.0.tar.gz
```

move **mips-decstation.linux-xgcc.gz** to root and untar

```shell=
sudo mv mips-decstation.linux-xgcc.gz /
cd /
sudo tar -zxvf mips-decstation.linux-xgcc.gz
```


---

### Install NachOS (Cont.)

Make NachOS-4.0
```shell=
cd ~/nachos-4.0/code
make
```


---

### Test

```shell=
cd ./userprog
./nachos -e ../test/test1
```


```
Total threads number is 1
Thread ../test/test1 is executing.
Print integer:9
Print integer:8
Print integer:7
Print integer:6
return value:0
```

