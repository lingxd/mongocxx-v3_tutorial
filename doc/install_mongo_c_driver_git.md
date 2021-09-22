# mongo-c-driver

```shell
$ git clone https://github.com/mongodb/mongo-c-driver.git
$ cd mongo-c-driver
$ git checkout 1.19.0  # To build a particular release
$ python build/calc_release_version.py > VERSION_CURRENT
$ mkdir cmake-build
$ cd cmake-build
$ cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..
```

[installing](http://mongoc.org/libmongoc/current/installing.html)

# mongo-cxx-driver

[mongocxx版本](https://github.com/mongodb/mongo-cxx-driver/releases) 

```shell
curl -OL https://github.com/mongodb/mongo-cxx-driver/releases/download/r3.6.5/mongo-cxx-driver-r3.6.5.tar.gz
tar -xzf mongo-cxx-driver-r3.6.5.tar.gz
cd mongo-cxx-driver-r3.6.5/build
```

## 配置驱动程序

在 Unix 系统上，默认`libmongoc`安装到`/usr/local`。没有额外的配置，`mongocxx`安装到它的本地构建目录，作为对那些从源代码构建的人的礼貌。要配置`mongocxx`安装到`/usr/local`，请使用以下 `cmake`命令：

```
cmake ..                                \
    -DCMAKE_BUILD_TYPE=Release          \
    -DCMAKE_INSTALL_PREFIX=/usr/local
```

如果安装到非标准目录（即动态加载程序不搜索的目录），请考虑指定`-DCMAKE_INSTALL_RPATH=`选项：

```
cmake ..                                \
    -DCMAKE_BUILD_TYPE=Release          \
    -DCMAKE_INSTALL_PREFIX=/opt/mongo   \
    -DCMAKE_INSTALL_RPATH=/opt/mongo
```

在随后的 Unix 示例中，`mongocxx`以以下方式自定义：

- `libmongoc`在 中找到`/opt/mongo-c-driver`。
- `mongocxx`是要安装到`/opt/mongo-cxx-driver`.

使用这两个不同的（任意）安装位置，用户将运行以下`cmake`命令：

```sh
cmake ..                                            \
    -DCMAKE_BUILD_TYPE=Release                      \
    -DCMAKE_PREFIX_PATH=/opt/mongo-c-driver         \
    -DCMAKE_INSTALL_PREFIX=/opt/mongo-cxx-driver
```

> *注意*如果您在 CMake PATH 变量中需要多个路径，请使用分号将它们分开，如下所示： `-DCMAKE_PREFIX_PATH="/your/cdriver/prefix;/some/other/path"`

这些选项可以与 C++17 polyfill 选项自由混合。例如，这是用户使用 Boost polyfill 选项运行上述命令的方式：

```sh
cmake ..                                            \
    -DCMAKE_BUILD_TYPE=Release                      \
    -DBSONCXX_POLY_USE_BOOST=1                      \
    -DCMAKE_PREFIX_PATH=/opt/mongo-c-driver         \
    -DCMAKE_INSTALL_PREFIX=/opt/mongo-cxx-driver
```

#### 配置`mongocxx`3.1.x 或 3.0.x

`-DCMAKE_PREFIX_PATH`用户必须`libmongoc`使用`-DLIBMONGOC_DIR`和`-DLIBBSON_DIR`选项指定安装目录，而不是选项：

```sh
cmake ..                                            \
    -DCMAKE_BUILD_TYPE=Release                      \
    -DLIBMONGOC_DIR=/opt/mongo-c-driver             \
    -DLIBBSON_DIR=/opt/mongo-c-driver               \
    -DCMAKE_INSTALL_PREFIX=/opt/mongo-cxx-driver
```

### 第 5 步：构建并安装驱动程序

如果您使用默认的 MNMLSTC polyfill 并安装到需要 root 权限的目录，您应该`sudo`在构建 mongocxx 的其余部分之前安装 polyfill，这样您就不必使用以下命令运行整个构建`sudo`：

```sh
# Only for MNMLSTC polyfill
sudo cmake --build . --target EP_mnmlstc_core
```

安装 MNMLSTC 后，或者如果您使用不同的 polyfill，请构建并安装驱动程序：

```sh
cmake --build .
sudo cmake --build . --target install
```

稍后可以通过以下两种方式之一卸载驱动程序。首先，可以调用卸载目标：

```sh
sudo cmake --build . --target uninstall
```

二、卸载脚本可以调用：

```sh
sudo /opt/mongo-cxx-driver/share/mongo-cxx-driver/uninstall.sh
```
