#include <M5Stack.h>
#include <HTTPClient.h>

String gethttp(String URL)
{
  HTTPClient http;
  http.begin(URL); //http://httpbin.org/ip
  String payload = "";
  char cpayload;
  if(http.GET() ==  HTTP_CODE_OK || http.GET() == HTTP_CODE_MOVED_PERMANENTLY) {
    payload = http.getString();
  }
  return payload;
}
