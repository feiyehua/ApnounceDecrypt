#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>

// 左移宏
#define LEFT_ROTATE(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

// SHA-1算法中的常量值
const uint32_t K[] = {
   0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6
};

// SHA-1算法中的循环函数
inline uint32_t f(uint32_t t, uint32_t B, uint32_t C, uint32_t D) {
   if (t < 20)
       return (B & C) | ((~B) & D);
   else if (t < 40)
       return B ^ C ^ D;
   else if (t < 60)
       return (B & C) | (B & D) | (C & D);
   else
       return B ^ C ^ D;
}

// 主要的SHA-1计算函数
std::string calculateSHA1(const std::string& hexInput) {
   // 将16进制字符串转换为字节序列
   std::vector<uint8_t> byteInput;
   for (size_t i = 0; i < hexInput.length(); i += 2) {
       std::string byteString = hexInput.substr(i, 2);
       byteInput.push_back(static_cast<uint8_t>(std::stoi(byteString, nullptr, 16)));
   }

   // 原始消息长度（以比特为单位）
   uint64_t originalMessageLength = byteInput.size() * 8;

   // 补充0x80（二进制的10000000）到消息末尾
   byteInput.push_back(0x80);

   // 填充0，直到满足 mod 512 ≡ 448
   while ((byteInput.size() * 8) % 512 != 448) {
       byteInput.push_back(0);
   }

   // 在消息的最后添加原始长度（64位）
   for (int i = 7; i >= 0; --i) {
       byteInput.push_back(static_cast<uint8_t>((originalMessageLength >> (i * 8)) & 0xFF));
   }

   // 初始化H0-H4
   uint32_t H[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};

   // 处理消息分块
   for (size_t i = 0; i < byteInput.size(); i += 64) {
       std::vector<uint8_t> block(byteInput.begin() + i, byteInput.begin() + i + 64);

       // 将每个块划分为16个32位字
       std::vector<uint32_t> W(80, 0);
       for (size_t j = 0; j < 16; ++j) {
           W[j] = (block[j * 4] << 24) | (block[j * 4 + 1] << 16) | (block[j * 4 + 2] << 8) | block[j * 4 + 3];
       }

       // 扩展 W 到 80 个字
       for (size_t j = 16; j < 80; ++j) {
           W[j] = LEFT_ROTATE(W[j - 3] ^ W[j - 8] ^ W[j - 14] ^ W[j - 16], 1);
       }

       // 初始化 A, B, C, D, E
       uint32_t A = H[0];
       uint32_t B = H[1];
       uint32_t C = H[2];
       uint32_t D = H[3];
       uint32_t E = H[4];

       // 主循环
       for (size_t j = 0; j < 80; ++j) {
           uint32_t temp = LEFT_ROTATE(A, 5) + f(j, B, C, D) + E + W[j] + K[j / 20];
           E = D;
           D = C;
           C = LEFT_ROTATE(B, 30);
           B = A;
           A = temp;
       }

       // 更新 H 值
       H[0] += A;
       H[1] += B;
       H[2] += C;
       H[3] += D;
       H[4] += E;
   }

   // 将 H0-H4 转换为字符串表示
   std::ostringstream sha1Stream;
   sha1Stream << std::hex << std::setfill('0');
   for (int i = 0; i < 5; ++i) {
       sha1Stream << std::setw(8) << H[i];
   }

   return sha1Stream.str();
}
void incrementHex(std::string &hexString) {
    // 将十六进制字符串转为整数
    unsigned long long intVal = std::stoull(hexString, nullptr, 16);

    // 加一
    intVal++;

    // 将新的整数转为十六进制字符串
    std::stringstream ss;
    ss << std::hex << intVal;
    hexString = ss.str();

    // 转换为大写形式
    //std::transform(hexString.begin(), hexString.end(), hexString.begin(), ::toupper);
}

std::string startPoint="13a54c0";
std::string goal="abc21d3f9d5d98ecae9f3d514475290171f2bbed";
int main() {
   
   std::cin>>startPoint;
   std::string hexInput = startPoint; // 替换为你的16进制数
   std::string sha1Result = calculateSHA1(hexInput);
   int cnt=0;
   freopen("result.txt","w",stdout);
   while(sha1Result!=goal)
   {
        cnt++;
        incrementHex(hexInput);
        std::string sha1Result = calculateSHA1(hexInput);
        if(cnt>=100000)
        {
            cnt=0;
            std::cout<<hexInput<<" "<<sha1Result<<std::endl;
        }
   }
   std::cout << "SHA-1 for " << hexInput << ": " << sha1Result << std::endl;
   //std::string a="fffffffffffffffe";
   //incrementHex(a);
   //std::cout<<a;
   return 0;
}