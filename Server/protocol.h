#ifndef PROTOCOL_H
#define PROTOCOL_H
typedef  unsigned int uint;
//一个是请求一个是响应
enum ENUM_MSG_TYPE{
    ENUM_MSG_TYPE_MIN = 0,
    ENUM_MSG_TYPE_REGIST_REQUEST,//注册请求
    ENUM_MSG_TYPE_REGIST_RESPEND,//注册回应

    ENUM_MSG_TYPE_LOGIN_REQUEST,//登录请求
    ENUM_MSG_TYPE_LOGIN_RESPEND,//登录回应
    //查找用户
    ENUM_MSG_TYPE_SELECT_USER_REQUEST,
    ENUM_MSG_TYPE_SELECT_USER_RESPEND,
    //在线用户
    ENUM_MSG_TYPE_ONLINE_USER_REQUEST,
    ENUM_MSG_TYPE_ONLINE_USER_RESPEND,
    //添加好友
    ENUM_MSG_TYPE_ADD_FRIEND_REQUEST,
    ENUM_MSG_TYPE_ADD_FRIEND_RESPEND,
    //同意添加好友
    ENUM_MSG_TYPE_ADD_FRIEND_AGREE_REQUEST,
    ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPEND,
    //刷线好友
    ENUM_MSG_TYPE_REFRESH_FRIEND_REQUEST,
    ENUM_MSG_TYPE_REFRESH_FRIEND_RESPEND,
    //删除好友
    ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST,
    ENUM_MSG_TYPE_DELETE_FRIEND_RESPEND,
    //聊天
    ENUM_MSG_TYPE_CHAT_REQUEST,
    ENUM_MSG_TYPE_CHAT_RESPEND,
    //创建文件
    ENUM_MSG_TYPE_MKDIR_REQUEST,
    ENUM_MSG_TYPE_MKDIR_RESPEND,
    //刷新文件
    ENUM_MSG_TYPE_FLUSH_FILE_REQUEST,
    ENUM_MSG_TYPE_FLUSH_FILE_RESPEND,
    //删除文件夹
    ENUM_MSG_TYPE_DELETE_DIR_REQUEST,
    ENUM_MSG_TYPE_DELETE_DIR_RESPEND,
    //重命名文件
    ENUM_MSG_TYPE_RENAME_FILE_REQUEST,
    ENUM_MSG_TYPE_RENAME_FILE_RESPEND,

    //移动文件
    ENUM_MSG_TYPE_MOVE_FILE_REQUEST,
    ENUM_MSG_TYPE_MOVE_FILE_RESPEND,

    //上传文件
    ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST,
    ENUM_MSG_TYPE_UPLOAD_FILE_RESPEND,

    //上传文件数据
    ENUM_MSG_TYPE_UPLOAD_FILE_DATA_REQUEST,
    ENUM_MSG_TYPE_UPLOAD_FILE_DATA_RESPEND,

    ENUM_MSG_TYPE_MAX =0x00fffff,
};


struct PDU {
    uint uiPDULen;  //总协议数据的长度
    uint uiMsgLen;  //实际消息的长度
    uint uiMsgType; //消息类型
    char caData[64];//参数
    char caMsg[];   //实际消息
};

struct FileInfo{
    uint FileType;
    char FileName[32];
};

//构建协议数据单元
PDU *mkPDU(uint uiMsgLen);


#endif // PROTOCOL_H
