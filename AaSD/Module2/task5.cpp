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

class Hasher {
    public:
        size_t operator()(const std::vector<byte>& codesSymbols) const {
            const size_t FNV_prime = 6547;
            const size_t FNV_offset_basis = 1163;

            size_t hash = FNV_offset_basis;

            for (byte bit : codesSymbols) {
                hash ^= static_cast<size_t>(bit);
                hash *= FNV_prime;
            }

            return hash;
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

void CreateResultRequest(std::vector<byte>& resultProcessedRequest,
    const std::vector<byte>& bitRepresentation, const std::vector<std::pair<int, byte>>& sortedLengthSymbols, byte quantitySymbols) {
        resultProcessedRequest.push_back(quantitySymbols);

        for (size_t i = 0; i < sortedLengthSymbols.size(); ++i) {
            resultProcessedRequest.push_back(static_cast<byte>(sortedLengthSymbols[i].first));
            resultProcessedRequest.push_back(sortedLengthSymbols[i].second);
        }

        for (size_t i = 0; i < bitRepresentation.size(); ++i) {
            resultProcessedRequest.push_back(std::move(bitRepresentation[i]));
        }
}

void CompressRequest(BitWriter& compresser, const std::vector<byte>& resultProcessedRequest, byte quantitySagnificantBits) {
    for (int i = 3; i >= 0; --i) {
        compresser.WriteBit(quantitySagnificantBits >> i & 1);
    }

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

    std::map<byte, size_t> tableCodeDigits;
    MakeTableLengthCodesDigits(tableCodeDigits, root);

    std::vector<std::pair<int, byte>> sortedCodesSymbol;
    for (const auto& [key, length] : tableCodeDigits) {
        sortedCodesSymbol.push_back({length, key});
    }

    std::sort(sortedCodesSymbol.begin(), sortedCodesSymbol.end());

    std::unordered_map<byte, std::vector<byte>> codesSymbols;
    MakeCodesSymbol(codesSymbols, sortedCodesSymbol);

    std::vector<byte> bitRepresentation;
    MakeBitRepresentation(bitRepresentation, codesSymbols, originalStr);

    byte quantitySymbols = static_cast<byte>(mapDigits.size() - 1);
    byte quantitySagnificantBits = static_cast<byte>((bitRepresentation.size() + 4) % 8);

    BitWriter compresser;
    CompressRequest(compresser, bitRepresentation, quantitySagnificantBits);

    std::vector<byte> compressedRequest = compresser.GetBuffer();

    std::vector<byte> resultProcessedRequest;
    CreateResultRequest(resultProcessedRequest, compressedRequest, sortedCodesSymbol, quantitySymbols);

    for (const auto& digit : resultProcessedRequest) {
        compressed.Write(digit);
    }

    CleanNodes(root);
}




void RecoverCodesSymbol(std::unordered_map<std::vector<byte>, byte, Hasher>& codesSymbols,
    std::vector<byte>& compressedRequest, size_t countSymbols, size_t& indexRequest) {
        int currentLength = 0;
        int currentCode = 0;

        for (size_t i = 0; i < countSymbols; ++i) {
            int len = static_cast<int>(compressedRequest[indexRequest++]);

            if (i > 0) {
                currentCode = (currentCode + 1) << (len - currentLength);
            } else {
                currentCode = 0;
            }

            currentLength = len;

            std::vector<byte> codeSymbol;

            for (int i = len - 1; i >= 0; --i) {
                byte bit = (currentCode >> i) & 1;

                codeSymbol.push_back(bit + '0');
            }

            codesSymbols[codeSymbol] = compressedRequest[indexRequest++];
        }
    }

void RecoverOriginal(std::vector<byte>& result, const std::vector<byte>& compressedRequest,
    std::unordered_map<std::vector<byte>, byte, Hasher>& codesSymbols, size_t indexRequest, size_t countSagnificantLastBits) {

    std::vector<byte> sequenceBit;

    for (size_t i = indexRequest; i < compressedRequest.size(); ++i) {
        int lastPos = 0;
        int beginPos = 7;

        if (i == compressedRequest.size() - 1 && countSagnificantLastBits != 0) {
            lastPos = 8 - countSagnificantLastBits;
        }

        if (i == indexRequest) {
            beginPos = 3;
        }


        for (int j = beginPos; j >= lastPos; --j) {
            byte bit = (compressedRequest[i] >> j) & 1;

            sequenceBit.push_back(bit + '0');

            if (codesSymbols.count(sequenceBit)) {
                result.push_back(codesSymbols[sequenceBit]);

                sequenceBit.clear();
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

    size_t countSymbols = static_cast<size_t>(compressedRequest[0]) + 1;

    size_t indexRequest = 1;

    std::unordered_map<std::vector<byte>, byte, Hasher> codesSymbols;
    RecoverCodesSymbol(codesSymbols, compressedRequest, countSymbols, indexRequest);

    byte byteCountSagnificantLastBits = 0;
    byteCountSagnificantLastBits |= compressedRequest[indexRequest] >> 4;

    size_t countSagnificantLastBits = static_cast<size_t>(byteCountSagnificantLastBits);

    std::vector<byte> result;
    RecoverOriginal(result, compressedRequest, codesSymbols, indexRequest, countSagnificantLastBits);

    for (const auto& digit : result) {
        original.Write(digit);
    }
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
