#include <bits/stdc++.h>
#include <ranges>
static std::array<uint64_t, 3> registers {};
static std::array<uint64_t, 16> program {};

uint64_t combo(uint64_t operand) {
    if (0 <= operand && operand <= 3) {
        return operand;
    }
    if (operand == 4) return registers[0];
    if (operand == 5) return registers[1];
    if (operand == 6) return registers[2];
    if (operand == 7) {
        std::cout << "Placeholder!\n";
        return 0;
    }
    return 0;
}

void dv(std::size_t register_index, uint64_t operand) {
    operand = combo(operand);
    if (operand >= 63) {
        std::cout << "Shifting by " << operand << " the result is illegal\n";
    }
    operand = std::pow(2, operand);
    registers[register_index] = registers[0] / operand;
}

void bxl(uint64_t operand) {
    registers[1] = static_cast<uint64_t>(registers[1]) ^ operand;
}

void bst(uint64_t operand) {
    registers[1] = combo(operand) % 8;
}

void jnz(uint64_t operand, int32_t &instruction_pointer) {
    if (registers[0] != 0) {
         instruction_pointer = operand;
    }
}

void bxc() {
    registers[1] = static_cast<uint64_t>(registers[1]) ^ static_cast<uint64_t>(registers[2]);
}

void out(uint64_t operand) {
    std::cout << combo(operand) % 8 << ",";
}

void execute_op(int64_t opcode, uint64_t operand, int &instruction_pointer) {
    switch (opcode) {
        case 0:
            dv(0, operand);
            return;
        case 1:
            bxl(operand);
            return;
        case 2:
            bst(operand);
            return;
        case 3:
            jnz(operand, instruction_pointer);
            return;
        case 4:
            bxc();
            return;
        case 5:
            out(operand);
            return;
        case 6:
            dv(1, operand);
            return;
        case 7:
            dv(2, operand);
            return;
    }
}

// -1 signifies failed attempt at finding a digit
uint64_t backtrack(int64_t current_program_digit, uint64_t previous_a) {
    if (current_program_digit < 0) return previous_a;
    std::array<uint64_t, 3> tmp_registers {};
    bool any_complete = false;
    for (int i = 0; i < 8; i++) {
        tmp_registers[0] = (8 * previous_a) + i; // Only 8 possible numbers due to remainder of division by 8 being 0 to 7
        tmp_registers[1] = tmp_registers[0] % 8;
        tmp_registers[1] ^= 3;
        tmp_registers[2] = tmp_registers[0] / pow(2, tmp_registers[1]);
        tmp_registers[0] /= 8;
        tmp_registers[1] ^= tmp_registers[2];
        tmp_registers[1] ^= 5;
        if ((tmp_registers[1] % 8) == program[current_program_digit]) {
            auto result = backtrack(current_program_digit - 1, previous_a * 8 + i);
            if (result != -1) {
                return result;
            }
        } else {
            continue;
        }
    }
    return -1;
}

int main() {
    std::string l;
    for (int i = 0; i < 3; i++) {
        std::getline(std::cin, l);
        registers[i] = std::stoll(l.substr(12));
    }
    std::getline(std::cin, l);
    std::getline(std::cin, l);
    std::string input = l.substr(9);
    int32_t instruction_pointer {0};
    for (auto const &op: std::views::split(input, ',')) {
        program[instruction_pointer] = op[0] - '0';
        instruction_pointer++;
    }

    instruction_pointer = 0;
    uint64_t opcode, operand;
    while (instruction_pointer < program.size()) {
        opcode = program[instruction_pointer++];
        if (instruction_pointer >= program.size()) {
            break;
        }
        operand = program[instruction_pointer++];
        execute_op(opcode, operand, instruction_pointer);
    }
    std::cout << "\n";
    uint64_t p2_result = backtrack(15, 0);
    std::cout << p2_result << "\n";
}
