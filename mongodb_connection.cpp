#include <cstdint>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <mongocxx/exception/exception.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

int main(int argc, char const *argv[])
{
    /**
     * @brief 建立联系
     * 1! 建立联系之前，首先要创建一个且仅一个`mongocxx::instance`，此实例必须存在于整个程序中。
     * 2! 请使用`mongocxx::client`去连接正在运行的`MongoDB`实例
     */
    mongocxx::instance instance{};                   // 一个且仅一个
    mongocxx::uri uri(mongocxx::uri::k_default_uri); // "mongodb://localhost:27017"
    mongocxx::client client(uri);

    /**
     * @brief 访问数据库
     * 1! 将mongocxx::client 实例连接到 MongoDB 部署后，请使用 database()方法 或operator[]获取 mongocxx::database 实例
     * 2! 如果请求的数据库不存在，MongoDB会在您第一次储存数据时创建数据库
     */
    mongocxx::database db = client["hokori"];

    /**
     * @brief 访问合集
     * 获得 mongocxx::database 实例后，请使用该collection()方法或operator[]获取 mongocxx::collection 实例。
     * 如果您请求的集合不存在，MongoDB 会在您第一次存储数据时创建它。
     * 下面的语句会访问 hokori 数据库中的 test 集合
     */
    mongocxx::collection coll = db["test"];

    /**
     * @brief 创建文档
     * 要document使用 C++ 驱动程序创建一个，请使用两个可用的构建器接口之一：
     * 流构建器：bsoncxx::builder::stream 使用流操作符的文档构建器，适用于文字文档构建。
     * 基本构建器：bsoncxx::builder::basic 一种更传统的文档构建器，涉及在构建器实例上调用方法。
     * 下面是流构建器：
     */

    // {
    //     "name" : "MongoDB",
    //     "type" : "database",
    //     "count" : 1,
    //     "versions" : [ "v3.2", "v3.0", "v2.6" ],
    //     "info":
    //     {
    //         "x" : 203,
    //         "y" : 102
    //     }
    // }

    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
                                         << "name"
                                         << "MongoDB"
                                         << "type"
                                         << "database"
                                         << "count" << 1
                                         << "versions" << bsoncxx::builder::stream::open_array
                                         << "v3.2"
                                         << "v3.0"
                                         << "v2.6"
                                         << close_array
                                         << "info" << bsoncxx::builder::stream::open_document
                                         << "x" << 203
                                         << "y" << 102
                                         << bsoncxx::builder::stream::close_document
                                         << bsoncxx::builder::stream::finalize;

    /**
    * @brief 使用 bsoncxx::builder::stream::finalize 去获取 bsoncxx::document::value 实例
    * bsoncxx::document::value 类型是一个只读的对象，拥有自己的内存
    * 需要使用的话，需要使用 bsoncxx::document::view 的 view() 方法
    */

    bsoncxx::document::view view = doc_value.view();

    /**
     * @brief 您可以使用 operator[] 访问此文档视图中的字段，这将返回一个 bsoncxx::document::element 实例。 
     * 例如，以下将提取值为字符串的 name 字段：
     * 
     */

    try
    {
        bsoncxx::document::element element = view["name"];
        if (element.type() != bsoncxx::type::k_utf8)
        {
            //Error
            std::cout << "is not bsoncxx::type::k_utf8" << std::endl;
        }

        std::string name = element.get_utf8().value.to_string();
        std::cout << "\"name\" : "
                  << "\"" << name << "\"" << std::endl;
    }
    catch (const mongocxx::exception &e)
    {
        std::cerr << "An exception occurred: " << e.what() << '\n';
    }

    /**
     * @brief 插入文档
     * 要将单个文档插入到集合中，请使用 mongocxx::collection 实例的 insert_one() 方法：
     */
    auto result = coll.insert_one(doc_value.view());

    return 0;
}
