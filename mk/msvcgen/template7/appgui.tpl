<?xml version="1.0" encoding = "Windows-1252"?>
<VisualStudioProject
	ProjectType="Visual C++"
	Version="7.00"
	Name="%project%"
	SccProjectName=""
	SccLocalPath="">
	<Platforms>
		<Platform
			Name="Win32"/>
	</Platforms>
	<Configurations>
		<Configuration
			Name="Release|Win32"
			OutputDirectory=".\release\temp\%project%"
			IntermediateDirectory=".\release\temp\%project%"
			ConfigurationType="1"
			UseOfMFC="0"
			ATLMinimizesCRunTimeLibraryUsage="FALSE">
			<Tool
				Name="VCCLCompilerTool"
				InlineFunctionExpansion="1"
				AdditionalIncludeDirectories="..\include,..\,..\..\CS\include"
				PreprocessorDefinitions="NDEBUG,_WINDOWS,_MT,WIN32,WIN32_VOLATILE,__CRYSTAL_SPACE__"
				StringPooling="TRUE"
				RuntimeLibrary="2"
				EnableFunctionLevelLinking="TRUE"
				UsePrecompiledHeader="2"
				PrecompiledHeaderFile=".\release\temp\%project%/%project%.pch"
				AssemblerListingLocation=".\release\temp\%project%/"
				ObjectFile=".\release\temp\%project%/"
				ProgramDataBaseFileName=".\release\temp\%project%/"
				WarningLevel="3"
				SuppressStartupBanner="TRUE"
				CompileAs="0"/>
			<Tool
				Name="VCCustomBuildTool"/>
			<Tool
				Name="VCLinkerTool"
				AdditionalOptions="/MACHINE:I386"
				AdditionalDependencies="libcssys.lib libcsutil.lib libcstool.lib libcsgeom.lib odbc32.lib odbccp32.lib"
				OutputFile=".\release\temp\%project%/%target%"
				LinkIncremental="1"
				SuppressStartupBanner="TRUE"
				AdditionalLibraryDirectories="..\..\CS\mk\visualc7\csrelease\bin\libs,..\..\CS\libs\cssys\win32\libs"
				IgnoreDefaultLibraryNames="LIBCMTD,LIBCMT"
				ProgramDatabaseFile=".\release\temp\%project%/%project%.pdb"
				SubSystem="2"/>
			<Tool
				Name="VCMIDLTool"
				PreprocessorDefinitions="NDEBUG"
				MkTypLibCompatible="TRUE"
				SuppressStartupBanner="TRUE"
				TargetEnvironment="1"
				TypeLibraryName=".\release\temp\%project%/%project%.tlb"/>
			<Tool
				Name="VCPostBuildEventTool"
				CommandLine="echo Moving output to CEL root.
copy &quot;$(TargetPath)&quot;  ..
"/>
			<Tool
				Name="VCPreBuildEventTool"/>
			<Tool
				Name="VCPreLinkEventTool"/>
			<Tool
				Name="VCResourceCompilerTool"
				PreprocessorDefinitions="NDEBUG"
				Culture="2057"/>
			<Tool
				Name="VCWebServiceProxyGeneratorTool"/>
			<Tool
				Name="VCWebDeploymentTool"/>
		</Configuration>
		<Configuration
			Name="Debug|Win32"
			OutputDirectory=".\debug\temp\%project%"
			IntermediateDirectory=".\debug\temp\%project%"
			ConfigurationType="1"
			UseOfMFC="0"
			ATLMinimizesCRunTimeLibraryUsage="FALSE">
			<Tool
				Name="VCCLCompilerTool"
				Optimization="0"
				AdditionalIncludeDirectories="..\include,..\,..\..\CS\include"
				PreprocessorDefinitions="_DEBUG,_MT,WIN32,_WINDOWS,WIN32_VOLATILE,__CRYSTAL_SPACE__,CS_DEBUG"
				BasicRuntimeChecks="3"
				RuntimeLibrary="3"
				UsePrecompiledHeader="2"
				PrecompiledHeaderFile=".\debug\temp\%project%/%project%.pch"
				AssemblerListingLocation=".\debug\temp\%project%/"
				ObjectFile=".\debug\temp\%project%/"
				ProgramDataBaseFileName=".\debug\temp\%project%/"
				WarningLevel="3"
				SuppressStartupBanner="TRUE"
				DebugInformationFormat="4"
				CompileAs="0"/>
			<Tool
				Name="VCCustomBuildTool"/>
			<Tool
				Name="VCLinkerTool"
				AdditionalOptions="/MACHINE:I386"
				AdditionalDependencies="libcssys_d.lib libcsutil_d.lib libcstool_d.lib libcsgeom_d.lib odbc32.lib odbccp32.lib"
				OutputFile=".\debug\temp\%project%/%target%"
				LinkIncremental="2"
				SuppressStartupBanner="TRUE"
				AdditionalLibraryDirectories="c:\kwf\prog\CS\mk\visualc7\csdebug\libs,c:\kwf\prog\CS\mk\visualc7\csdebug\bin\libs"
				GenerateDebugInformation="TRUE"
				ProgramDatabaseFile=".\debug\temp\%project%/%project%.pdb"
				SubSystem="2"/>
			<Tool
				Name="VCMIDLTool"
				PreprocessorDefinitions="_DEBUG"
				MkTypLibCompatible="TRUE"
				SuppressStartupBanner="TRUE"
				TargetEnvironment="1"
				TypeLibraryName=".\debug\temp\%project%/%project%.tlb"/>
			<Tool
				Name="VCPostBuildEventTool"
				CommandLine="echo Moving output to CEL root.
copy &quot;$(TargetPath)&quot;  ..
"/>
			<Tool
				Name="VCPreBuildEventTool"/>
			<Tool
				Name="VCPreLinkEventTool"/>
			<Tool
				Name="VCResourceCompilerTool"
				PreprocessorDefinitions="_DEBUG"
				Culture="2057"/>
			<Tool
				Name="VCWebServiceProxyGeneratorTool"/>
			<Tool
				Name="VCWebDeploymentTool"/>
		</Configuration>
	</Configurations>
	<Files>
		%groups%
	</Files>
	<Globals>
	</Globals>
</VisualStudioProject>
