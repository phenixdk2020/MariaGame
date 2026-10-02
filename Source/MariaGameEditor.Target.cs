using UnrealBuildTool;
using System.Collections.Generic;

public class MariaGameEditorTarget : TargetRules
{
    public MariaGameEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("MariaGame");
    }
}
