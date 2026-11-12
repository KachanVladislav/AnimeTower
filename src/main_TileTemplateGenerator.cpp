#include "atTileTemplateGenerator/GridTileTemplateGenerator.h"

bool dirResult_Initialize(const char *dirResults)
{
    if (!DirectoryExists(dirResults))
        return (MakeDirectory(dirResults) == 0);
    return true;
}


int main()
{
    const char *dirResults = "./tileTemplates/";
    dirResult_Initialize(dirResults);

    
    GridTileTemplateSettings settingsPlatform[] = {
        GridTileTemplateSettingsBuilder::platform(),
    };
    for(const auto& setting: settingsPlatform)
        GridTileTemplateGenerator::generate(dirResults, "tileTemplatePlatform", setting);

    GridTileTemplateSettings settingsGrid[] = {
        GridTileTemplateSettingsBuilder::grid_2_2_64_128(),
        GridTileTemplateSettingsBuilder::grid(3, 3, 64, 128)
    };
    for(const auto& setting: settingsGrid)
        GridTileTemplateGenerator::generate(dirResults, "tileTemplateGrid", setting);

    return 0;
}
