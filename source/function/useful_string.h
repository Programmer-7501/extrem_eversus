/**
* @file useful_string.h
* @brief string便利関数
*/

#pragma once

/**
* @brief 拡張子取得関数
* @param[in] const std::string&(fileName) ファイルネーム
* @param[out] std::string&(out) 拡張子
* @return bool 切り取りが成功したかどうか
*/
bool ExtractFileExpand(const std::string& fileName, std::string& out);

/**
* @brief 切り取り
* @param[in] const std::string&(input) 入力
* @param[in] char(delimiter) 切り取る文字
* @return std::vector<std::string> 切り取り後の文字列
*/
std::vector<std::string> Split(std::string& input, char delimiter);