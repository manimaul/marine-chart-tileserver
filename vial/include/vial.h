#pragma once

#define VIAL_VERSION "0.0.1_SNAPSHOT"

#include "Config.h"
#include "HttpMessage.h"
#include "HttpMethod.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpServer.h"
#include "HttpStatus.h"
#include "LambdaRequestHandler.h"
#include "LambdaRequestHandlerFactory.h"
#include "StaticContentHandlerFactory.h"
#include "StaticHandler.h"

namespace vial {
    class Init {
    public:
        static void init(int* argc, char*** argv);
    };
}
