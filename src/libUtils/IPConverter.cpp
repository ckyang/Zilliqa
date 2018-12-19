/*
 * Copyright (C) 2019 Zilliqa
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "IPConverter.h"

using namespace std;

namespace LogInfo {
void LogBrand() {
  cout << "Copyright (C) Zilliqa. Version 3.0 (Durian - Mao Shan Wang). "
          "<https://www.zilliqa.com/> "
       << endl;
}

void LogBugReport() {
  cout << "For bug reporting, please create an issue at "
          "<https://github.com/Zilliqa/Zilliqa> \n"
       << endl;
}
void LogBrandBugReport() {
  LogBrand();
  LogBugReport();
}
}  // namespace LogInfo

namespace IPConverter {
using namespace LogInfo;

const std::string ToStrFromNumericalIP(
    const boost::multiprecision::uint128_t& ip) {
  char str[INET_ADDRSTRLEN];
  struct sockaddr_in serv_addr;
  serv_addr.sin_addr.s_addr = ip.convert_to<unsigned long>();
  inet_ntop(AF_INET, &(serv_addr.sin_addr), str, INET_ADDRSTRLEN);
  return std::string(str);
}

void LogUnsupported(const string& ip) {
  LogBrandBugReport();
  cerr << "Error: Unknown address type " << ip << ", unsupported protocol\n"
       << endl;
}

void LogInvalidIP(const string& ip) {
  LogBrandBugReport();
  cerr << "Error: address " << ip
       << " does not contain a character string "
          "representing a valid network address\n"
       << endl;
}

void LogInternalErr(const string& ip) {
  LogBrandBugReport();
  cerr << "Internal Error: cannot process the input IP address " << ip << ".\n"
       << std::endl;
}

int ToNumericalIPFromStr(const std::string& ipStr,
                         boost::multiprecision::uint128_t& ipInt) {
  boost::asio::ip::address Addr;
  try {
    Addr = boost::asio::ip::address::from_string(ipStr);
  } catch (const std::exception& e) {
    LogInvalidIP(ipStr);
    return -1;
  }
  ipInt = 0;
  if (Addr.is_v4()) {
    ipInt = convertBytesToInt(Addr.to_v4());
    return 0;
  } else if (Addr.is_v6()) {
    ipInt = convertBytesToInt(Addr.to_v6());
    return 0;
  }

  LogUnsupported(ipStr);
  return -1;
}
}  // namespace IPConverter
