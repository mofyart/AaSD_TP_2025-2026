#include "Huffman.h"

#include <set>
#include <unordered_map>
#include <stack>
#include <vector>
#include <algorithm>
#include <map>


struct Node {
    int frequencyOccurrence;
    byte digit;
    Node* right;
    Node* left;
};

typedef unsigned char byte;

class BitWriter {
    private:
        size_t bitCount;
        std::vector<byte> buffer;

    public:
        BitWriter() : bitCount(0) {}

        void WriteBit(byte bit) {
            if (bitCount % 8 == 0) {
                buffer.push_back(0);
            }

            if (bit) {
                buffer[bitCount / 8] |= 1 << (7 - bitCount % 8);
            }

            ++bitCount;
        }

        void WriteByte(byte byteVal) {
            if (bitCount % 8 == 0) {
                buffer.push_back(byteVal);
            } else {
                int offset = bitCount % 8;

                buffer[bitCount / 8] |= byteVal >> offset;

                buffer.push_back(byteVal << (8 - offset));
            }

            bitCount += 8;
        }

        const std::vector<byte>& GetBuffer() {
            return buffer;
        }

        const size_t GetBitCount() {
            return bitCount;
        }
};

void CleanNodes(Node* node) {
    if (!node) {
        return;
    }

    CleanNodes(node->left);
    CleanNodes(node->right);

    delete node;
}


void FillMinHeapFrequencesSymbols(std::set<std::pair<int, Node*>>& minHeap, const std::unordered_map<byte, int>& mapDigits) {
    for (auto& [key, val] : mapDigits) {
        minHeap.insert({val, new Node{val, key}});
    }
}

Node* MakeKaffmanTree(std::set<std::pair<int, Node*>>& minHeap) {
    Node* root;

    while (!minHeap.empty()) {
        std::pair<int, Node*> rightNode = *(minHeap.begin());
        minHeap.erase(minHeap.begin());

        std::pair<int, Node*> leftNode;

        if (!minHeap.empty()) {
            leftNode = *(minHeap.begin());
            minHeap.erase(minHeap.begin());

            minHeap.insert({
                leftNode.first + rightNode.first,
                new Node{leftNode.first + rightNode.first,
                '\0',
                rightNode.second,
                leftNode.second}});
        } else {
            root = rightNode.second;
        }
    }

    return root;
}

void MakeBitRepresentation(std::vector<byte>& bitRepresentation, std::unordered_map<byte, std::vector<byte>>& tableCodesDigits, std::vector<byte>& originalStr) {
    for (const byte& digit : originalStr) {
        for (size_t i = 0; i < tableCodesDigits[digit].size(); ++i) {
            bitRepresentation.push_back(tableCodesDigits[digit][i]);
        }
    }
}

void MakeTableLengthCodesDigits(std::map<byte, size_t>& tableCodesDigits, Node* root) {
    std::stack<Node*> nodesStack;

    Node* lastVisited = nullptr;

    Node* curNode = root;

    size_t lengthCode = 0;

    while(!nodesStack.empty() || curNode) {
        if (curNode) {
            if (curNode != root) {
                ++lengthCode;
            }

            nodesStack.push(curNode);

            curNode = curNode->left;
        } else {
            Node* topNode = nodesStack.top();

            if (topNode->right && lastVisited != topNode->right) {
                curNode = topNode->right;
                nodesStack.push(curNode);
                ++lengthCode;

                curNode = curNode->left;
            } else {
                if (!topNode->left && !topNode->right) {
                    tableCodesDigits[topNode->digit] = lengthCode;
                }

                nodesStack.pop();
                lastVisited = topNode;

                if (!nodesStack.empty()) {
                    --lengthCode;
                }
            }
        }
    }
}

void CompressRequest(BitWriter& compresser, const std::vector<byte>& resultProcessedRequest, const std::vector<std::pair<int, byte>>& sortedLengthSymbols, byte quantitySagnificantBits) {
    // первый байт займёт максимальная длина кода символа
    compresser.WriteByte(static_cast<byte>(sortedLengthSymbols[sortedLengthSymbols.size() - 1].first));

    int count = 0;
    int length = 1;

    // для каждой возможной длины кода до максимальной
    // длины мы заполняем количестовм символов имеющую данную длину

    //!!! Заметим, что мы начинаем записовать со 2-го байта,
    // следовательно индекс 2 - 1 = 1 , а это = минимальная длина кода символа

    for (int i = 0; i < sortedLengthSymbols.size(); ++i) {
        if (length != sortedLengthSymbols[i].first) {
            compresser.WriteByte(static_cast<byte>(count));

            --i;

            count = 0;
            ++length;
        } else {
            ++count;
        }
    }

    // пишем количество символов имеющую максимальную длину волны
    compresser.WriteByte(static_cast<byte>(count));

    // пишем сами символы
    for (size_t i = 0; i < sortedLengthSymbols.size(); ++i) {
        compresser.WriteByte(sortedLengthSymbols[i].second);
    }

    // записываю количество значимых битов в последнем байте, при помощи 3 битов
    // (так как 7 = 111 максимум 3 бита)
    for (int i = 2; i >= 0; --i) {
        compresser.WriteBit(quantitySagnificantBits >> i & 1);
    }

    // записываем в битовом представлении закодированную строку
    for (size_t i = 0; i < resultProcessedRequest.size(); ++i) {
        compresser.WriteBit(resultProcessedRequest[i] - '0');
    }
}

void MakeCodesSymbol(std::unordered_map<byte, std::vector<byte>>& codesSymbols, const std::vector<std::pair<int, byte>>& sortedLength) {
    int currentCode = 0;
    int currentLength = 0;

    for (size_t i = 0; i < sortedLength.size(); ++i) {
        int len = sortedLength[i].first;

        if (i > 0) {
            currentCode = (currentCode + 1) << (len - currentLength);
        } else {
            currentCode = 0;
        }

        currentLength = len;

        for (int j = len - 1; j >= 0; --j) {
            byte bit = (currentCode >> j) & 1;

            codesSymbols[sortedLength[i].second].push_back(bit + '0');
        }
    }
}

void Encode(IInputStream& original, IOutputStream& compressed) {
    byte digit;

    std::set<std::pair<int, Node*>> minHeap;
    std::unordered_map<byte, int> mapDigits;
    std::vector<byte> originalStr;

	while (original.Read(digit)) {
        originalStr.push_back(digit);
        ++mapDigits[digit];
	}

    if (originalStr.size() == 0) {
        return;
    }

    FillMinHeapFrequencesSymbols(minHeap, mapDigits);

    Node* root = MakeKaffmanTree(minHeap);

    // считаем длину кодов для каждого символа
    std::map<byte, size_t> tableCodeDigits;
    MakeTableLengthCodesDigits(tableCodeDigits, root);

    std::vector<std::pair<int, byte>> sortedCodesSymbol;
    for (const auto& [key, length] : tableCodeDigits) {
        sortedCodesSymbol.push_back({length, key});
    }

    // сортируем в начале по длине, потом по алфавиту
    std::sort(sortedCodesSymbol.begin(), sortedCodesSymbol.end());

    // для каждого символа создаём код, но теперь через Канонический алгоритм Хафмана
    std::unordered_map<byte, std::vector<byte>> codesSymbols;
    MakeCodesSymbol(codesSymbols, sortedCodesSymbol);

    // в соотвествии с кодами для каждго символа мы кодируем нашу строку
    std::vector<byte> bitRepresentation;
    MakeBitRepresentation(bitRepresentation, codesSymbols, originalStr);

    // количество значимых бит
    byte quantitySagnificantBits = static_cast<byte>((bitRepresentation.size() + 3) % 8);

    // сжимаем наш исходный текст и добаляем дополнительную инфлрмацию
    BitWriter compresser;
    CompressRequest(compresser, bitRepresentation, sortedCodesSymbol, quantitySagnificantBits);

    std::vector<byte> compressedRequest = compresser.GetBuffer();

    for (const auto& digit : compressedRequest) {
        compressed.Write(digit);
    }

    CleanNodes(root);
}


Node* RecoverCodesSymbol(std::vector<byte>& compressedRequest, size_t countCounter, size_t& indexRequest) {
        int currentLength = 0;
        int currentCode = 0;

        size_t beginIndex = indexRequest;

        // создаём корень
        Node* root = new Node{0, '\0', nullptr, nullptr};

        // смещение относительно начального beginIndex, для того, чтобы
        // при получении кода символа в виде битового представления currentCode,
        // мы могли обращаться к символу, которому принадлежит данный код.
        // (описание для offset)
        size_t offset = countCounter;

        for (size_t i = indexRequest; i < countCounter + 1; ++i) {
            for (size_t j = 0; j < static_cast<size_t>(compressedRequest[i]); ++j) {
                // i - помним это длина кода как раз. Было пояснение в Encode.
                int len = i;

                if (i == beginIndex && j == 0) {
                    // первый код всегда 0.
                    currentCode = 0;
                } else {
                    // получение кода символа через каноничесий алгоритм Хафмана
                    currentCode = (currentCode + 1) << (len - currentLength);
                }


                currentLength = len;

                Node* curNode = root;

                // проходимся по битовому представлению числа, у которого как
                // его битовая последовательность и есть наш код символа
                // и воссоздаём дерево
                for (int i = len - 1; i >= 0; --i) {
                    byte bit = (currentCode >> i) & 1;

                    if (bit + '0' == '0') {
                        if (i == 0) {
                            curNode->right = new Node{
                                0,
                                compressedRequest[indexRequest + offset++],
                                nullptr,
                                nullptr
                            };
                            break;
                        } else {
                            if (!curNode->right) {
                                curNode->right = new Node{
                                    0,
                                    '\0',
                                    nullptr,
                                    nullptr
                                };
                            }
                        }

                        curNode = curNode->right;
                    } else {
                        if (i == 0) {
                            curNode->left = new Node{
                                0,
                                compressedRequest[indexRequest + offset++],
                                nullptr,
                                nullptr
                            };
                            break;
                        } else {
                            if (!curNode->left) {
                                curNode->left = new Node{
                                    0,
                                    '\0',
                                    nullptr,
                                    nullptr
                                };
                            }
                        }

                        curNode = curNode->left;
                    }
                }
            }
        }

        // для дальнейшего чтения кодированных данных
        indexRequest += offset;

        return root;
    }

void RecoverOriginal(std::vector<byte>& result, const std::vector<byte>& compressedRequest,
    Node* root, size_t indexRequest, size_t countSagnificantLastBits) {

    std::vector<byte> sequenceBit;
    Node* curNode = root;
    for (size_t i = indexRequest; i < compressedRequest.size(); ++i) {
        int lastPos = 0;
        int beginPos = 7;

        if (i == compressedRequest.size() - 1 && countSagnificantLastBits != 0) {
            lastPos = 8 - countSagnificantLastBits;
        }

        // так как первые 3 бита помним - это число
        // значащих битов в последнем байте
        if (i == indexRequest) {
            beginPos = 4;
        }

        for (int j = beginPos; j >= lastPos; --j) {
            if (!((compressedRequest[i] >> j) & 1)) {
                curNode = curNode->right;

                if (!curNode->left && !curNode->right) {
                    result.push_back(curNode->digit);

                    curNode = root;
                }
            } else {
                curNode = curNode->left;

                if (!curNode->left && !curNode->right) {
                    result.push_back(curNode->digit);

                    curNode = root;
                }
            }
        }
    }
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    std::vector<byte> compressedRequest;

    byte digit;

    while (compressed.Read(digit)) {
        compressedRequest.push_back(digit);
	}

    if (compressedRequest.empty()) return;

    // в начале сжатых данных будет количество счётчиков,
    // то есть количество длин кодов символов
    size_t countCounter = static_cast<size_t>(compressedRequest[0]);

    size_t indexRequest = 1;

    // строим дерево для дальнейшей декодировки
    Node* root = RecoverCodesSymbol(compressedRequest, countCounter, indexRequest);

    byte byteCountSagnificantLastBits = 0;
    // получаем символ с кодом ASCII равным числу значащих битов в последнем байте
    byteCountSagnificantLastBits |= compressedRequest[indexRequest] >> 5;

    size_t countSagnificantLastBits = static_cast<size_t>(byteCountSagnificantLastBits);

    // декодируем при помощи дерева Хаффмана
    std::vector<byte> result;
    RecoverOriginal(result, compressedRequest, root, indexRequest, countSagnificantLastBits);

    for (const auto& digit : result) {
        original.Write(digit);
    }

    CleanNodes(root);
}


int main() {
    const std::string originalFile = "input.txt";
    const std::string compressedFile = "comp.bin";
    const std::string outputFile = "output.txt";

    {
        FileInputStream in(originalFile);
        FileOutputStream out(compressedFile);

        Encode(in, out);
    }

    {
        FileInputStream in2(compressedFile);
        FileOutputStream out2(outputFile);

        Decode(in2, out2);
    }

    return 0;
}
