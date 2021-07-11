#include "HtmlTemplate.h"

std::string HtmlTemplate::ShowErrorPage(const std::string &code,
										const std::string &textCode) {
	std::string result;

	result = "Content-type:text/html\r\n\r\n";
	result += "<!DOCTYPE html>\n";
	result += "<html lang=\"en\">\n";
	result += "<head>\n";
	result += "<meta charset=\"UTF-8\">\n";
	result += "<title>Error " + code + "</title>\n";
	result += "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-+0n0xVW2eSR5OomGNYDnhzAbDsOXxcvSN1TPprVMTNDbiYZCxYbOOl7+AMvyTG2x\" crossorigin=\"anonymous\">\n";
	result += "</head>\n";
	result += "<body>\n";
	result += "<div class=\"container\">\n";
	result += "<div class=\"alert alert-danger d-flex align-items-center mt-5\" role=\"alert\">\n";
	result += "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"24\" height=\"24\" fill=\"currentColor\" class=\"bi bi-exclamation-triangle-fill flex-shrink-0 me-2\" viewBox=\"0 0 16 16\" role=\"img\" aria-label=\"Warning:\">\n";
	result += "<path d=\"M8.982 1.566a1.13 1.13 0 0 0-1.96 0L.165 13.233c-.457.778.091 1.767.98 1.767h13.713c.889 0 1.438-.99.98-1.767L8.982 1.566zM8 5c.535 0 .954.462.9.995l-.35 3.507a.552.552 0 0 1-1.1 0L7.1 5.995A.905.905 0 0 1 8 5zm.002 6a1 1 0 1 1 0 2 1 1 0 0 1 0-2z\"/>\n";
	result += "</svg>\n";
	result += "<div>\n<h3 class=\"mb-0\">" + textCode + "</h3>\n</div>\n";
	result += "</div>\n";
	result += "<div class=\"dropdown-divider mt-3\"></div>\n";
	result += "<div class=\"row mt-3\">\n";
	result += "<div class=\"col\"></div>\n";
	result += "<div class=\"col-auto\">\n<h5>Error " + code + " / Webserv</h5>\n</div>\n";
	result += "<div class=\"col\"></div>\n";
	result += "</div>\n</div>\n</body>\n</html>\n";

	return (result);
}

std::string HtmlTemplate::ShowListPage() {
	std::string result;

	result = "Content-type:text/html\r\n\r\n";
	result += "<!DOCTYPE html>\n";
	result += "<html lang=\"en\">\n";
	result += "<head>\n";
	result += "<meta charset=\"UTF-8\">\n";
	result += "<title>Autoindex</title>\n";
	result += "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-+0n0xVW2eSR5OomGNYDnhzAbDsOXxcvSN1TPprVMTNDbiYZCxYbOOl7+AMvyTG2x\" crossorigin=\"anonymous\">\n";
	result += "</head>\n";
	result += "<body>\n";
	result += "<div class=\"container\">\n";
	result += "<table class=\"table mt-5\">\n";
	result += "<thead><tr><th scope=\"col\">Name</th></tr></thead>\n";
	result += "<tbody>\n";

	//  Тут идет цикл файлов и папок

	result += "</tbody>\n";
	result += "</table>\n";
	result += "</div>\n";
	result += "</body>\n";
	result += "</html>\n";

	return (result);
}
