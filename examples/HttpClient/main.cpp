#include "http/client/httpclientimpl.h"
#include "logger/logger.h"
#include <QCoreApplication>
#include <QTimer>
#include <iostream>
int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  HttpClientImpl cli;
  Url url{"192.168.24.187:8080"};
  HttpResponse rsp = cli.requestHttpGET(url, "/api/v1/measurements");
  if (rsp.error() == Error::Success) {
    std::cout << "res->body" << rsp->body << " error "
              << static_cast<uint32_t>(rsp.error());
  } else {
    std::cout << "dont receive response " << static_cast<uint32_t>(rsp.error());
  }
  return a.exec();
}
