//------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

#include "STB/ConsoleApp.h"
#include "STB/Option.h"
#include "STB/XML.h"
#include "STB/Http.h"


#define  PROGRAM         "scrape"
#define  DESCRIPTION     "Scrape IF archive using http for test files (please be nice!)"
#define  LINK            "https://github.com/AnotherJohnH/"
#define  COPYRIGHT_YEAR  "2018"
#define  AUTHOR          "John D. Haughton"
#define  VERSION         PROJ_VERSION


class ScrapeApp : public STB::ConsoleApp
{
private:
   STB::Option<const char*> host{   'H', "host",  "Remote hostname",  "ifarchive.org"};
   STB::Option<const char*> path{   'P', "path",  "Remote path",      "/if-archive/games/zcode/"};
   STB::Option<const char*> ext{    'E', "ext",   "File extension",   "z5"};
   STB::Option<const char*> cache{  'C', "cache", "Cache directory",  "cache"};
   STB::Option<const char*> list_fn{'L', "list",  "List output file", "list.json"};

   void findLinks(STB::Http&               http,
                  const STB::XML::Element& xml,
                  PLT::File&               list,
                  bool&                    first)
   {
      for(const auto& element : xml)
      {
         if (element.getName() == "a")
         {
            std::string href = element["href"];

            // TODO handle hrefs that include a path

            std::string dot_ext = ".";
            dot_ext += ext;

            if (href.find(dot_ext) != std::string::npos)
            {
               std::string local_path = (const char*)path;
               local_path += href;

               std::string file = (const char*) cache;
               file += '/';
               file += href;

               if (PLT::File::size(file.c_str()) == 0)
               {
                  printf("GET http://%s%s as \"%s\"",
                         (const char*)host, local_path.c_str(), file.c_str());
                  bool ok = http.getFile(local_path, file);
                  printf(" - %s\n", ok ? "OK" : "FAIL");
              }

              if (first)
              {
                 first = false;
              }
              else
              {
                 list.printf(",\n");
              }

               list.printf("   [\"%s\"]", file.c_str());
            }
         }

         findLinks(http, element, list, first);
      }
   }

   virtual int startConsoleApp() override
   {
      STB::Http http;

      if(http.open((const char*)host))
      {
         std::string file = (const char*) cache;
         file += '/';
         file += "index.html";

         if(http.getFile((const char*)path, file))
         {
            STB::XML::Document xml(file, /* require_prolog */ false);
            PLT::File          list(nullptr, list_fn);

            if (list.openForWrite())
            {
               list.printf("[\n");

               bool first = true;
               findLinks(http, xml, list, first);

               list.printf("\n]\n");
            }
         }

         http.close();
      }

      return 0;
   }

public:
   ScrapeApp(int argc, const char* argv[])
      : ConsoleApp(PROGRAM, DESCRIPTION, LINK, AUTHOR, VERSION, COPYRIGHT_YEAR)
   {
      parseArgsAndStart(argc, argv);
   }
};


int main(int argc, const char* argv[])
{
   ScrapeApp(argc, argv);
}
