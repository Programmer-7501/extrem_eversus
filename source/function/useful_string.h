/**
* @file useful_string.h
* @brief string�֗��֐�
*/

#pragma once

/**
* @brief �g���q�擾�֐�
* @param[in] const std::string&(fileName) �t�@�C���l�[��
* @param[out] std::string&(out) �g���q
* @return bool �؂��肪�����������ǂ���
*/
bool ExtractFileExpand(const std::string& fileName, std::string& out);

/**
* @brief �؂���
* @param[in] const std::string&(input) ����
* @param[in] char(delimiter) �؂��镶��
* @return std::vector<std::string> �؂����̕�����
*/
std::vector<std::string> Split(std::string& input, char delimiter);