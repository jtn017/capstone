  % Autogenerated file: Sun May 29 15:42:47 2022
  function writeCodeInfoFcn


  % Load a ComponentInterface object from IR
  codeIRInfoStruct = load('tlc\codeIRInfo.mat');
codeInfo = codeIRInfoStruct.codeInfo;

  prmIdxMap = containers.Map;




  wr = coder.internal.writeDataInterfacesToCodeDescriptor("V2X_RX_Baseband",pwd) ;
  wr.writeName(codeInfo.Name);

  
  
  skippedParameters = [];
  
  


  
  






























































          tmpArgs         = [];
    tmpActualArgs   = [];
    tmpRet          = coder.types.Argument.empty;
    tmpActualReturn = RTW.DataInterface.empty;

        

            % Opaque Data Type
            type_RTModel  = coder.types.Opaque;
            type_RTModel.Identifier = 'RT_MODEL';
            

        % Pointer Type: Pointer to type_RTModel
        Ptrtype_RTModel_1_0 = coder.types.Pointer;
            Ptrtype_RTModel_1_0.BaseType = type_RTModel;
        Ptrtype_RTModel_1_0.ReadOnly = 1;
        Ptrtype_RTModel_1_0.Volatile = 0;
        

        % Variable: Var_RTModel
        Var_RTModel = RTW.Variable(type_RTModel, 'rtM_');
        Var_RTModel.Owner           = '';
        Var_RTModel.DeclarationFile = '';
        

        % Pointer Variable: PtrVar_RTModel
        PtrVar_RTModel = RTW.PointerVariable(Ptrtype_RTModel_1_0, 'rtM', Var_RTModel);
        

        % Data Interface: idata_RTModel
            empty_timing = RTW.TimingInterface.empty;
        idata_RTModel = RTW.DataInterface('', 'RTModel', PtrVar_RTModel, empty_timing);
        
            writeCodeInfoFcn_RTArg = coder.types.Argument;
            writeCodeInfoFcn_RTArg.Type   = Ptrtype_RTModel_1_0;
            writeCodeInfoFcn_RTArg.Name   = 'rtM';
            writeCodeInfoFcn_RTArg.IOType = 'INPUT_OUTPUT';

            tmpArgs    = [tmpArgs, writeCodeInfoFcn_RTArg];
            tmpActualArgs = [tmpActualArgs, idata_RTModel];


            % Opaque Data Type
            type_DWork  = coder.types.Opaque;
            type_DWork.Identifier = 'DW';
            

        % Pointer Type: Pointer to type_DWork
        Ptrtype_DWork_0_0 = coder.types.Pointer;
            Ptrtype_DWork_0_0.BaseType = type_DWork;
        Ptrtype_DWork_0_0.ReadOnly = 0;
        Ptrtype_DWork_0_0.Volatile = 0;
        

        % Variable: Var_rt_DWork
        Var_rt_DWork = RTW.Variable(type_DWork, 'rtDW');
        Var_rt_DWork.Owner           = '';
        Var_rt_DWork.DeclarationFile = '';
        


        % Structure Element: StructElem_PointerExpr_rt_DWork
        StructElem_PointerExpr_rt_DWork = coder.types.AggregateElement;
        StructElem_PointerExpr_rt_DWork.Identifier = 'dwork';
        StructElem_PointerExpr_rt_DWork.Type = Ptrtype_DWork_0_0;


        % Pointer Expression: PointerExpr_rt_DWork
        PointerExpr_rt_DWork = RTW.PointerExpression(StructElem_PointerExpr_rt_DWork, PtrVar_RTModel, Var_rt_DWork);
        



        % Data Interface: idata_rt_DWork
            empty_timing = RTW.TimingInterface.empty;
        idata_rt_DWork = RTW.DataInterface('', 'Block states', PointerExpr_rt_DWork, empty_timing);
        
                            
    % Built-In Type: 8
        Type_8_R0_V0_C0 = coder.types.Bool;
        Type_8_R0_V0_C0.Name = 'boolean';
    Type_8_R0_V0_C0.Identifier = 'boolean_T';
    Type_8_R0_V0_C0.ReadOnly   = 0;
    Type_8_R0_V0_C0.Volatile   = 0;
        
    % Matrix Type
    Type_19_R0_V0_C0 = coder.types.Matrix;
    Type_19_R0_V0_C0.BaseType   = Type_8_R0_V0_C0;
        Type_19_R0_V0_C0.Dimensions = [1976,1];
        Type_19_R0_V0_C0.SymbolicDimensions = Type_19_R0_V0_C0.SymbolicDimensions;
                                Initialize_EI0Arg = coder.types.Argument;
                        Initialize_EI0Arg.Type = Type_19_R0_V0_C0;
                        Initialize_EI0Arg.Name = 'rtU_v2x_rx_bb_in';
                        codeInfo.Inports(0+1).Implementation.Identifier = 'rtU_v2x_rx_bb_in';
                        tmpArgs    = [tmpArgs, Initialize_EI0Arg];
                        tmpActualArgs = [tmpActualArgs, codeInfo.Inports(0+1)];
                        wr.updateInportArg(codeInfo.Inports(0+1), 0+1);
                            
    % Built-In Type: 3
        Type_3_R0_V0_C0 = coder.types.Int(8,0);
        Type_3_R0_V0_C0.Name = 'uint8';
    Type_3_R0_V0_C0.Identifier = 'uint8_T';
    Type_3_R0_V0_C0.ReadOnly   = 0;
    Type_3_R0_V0_C0.Volatile   = 0;
        
    % Matrix Type
    Type_20_R0_V0_C0 = coder.types.Matrix;
    Type_20_R0_V0_C0.BaseType   = Type_3_R0_V0_C0;
        Type_20_R0_V0_C0.Dimensions = [98,1];
        Type_20_R0_V0_C0.SymbolicDimensions = Type_20_R0_V0_C0.SymbolicDimensions;
                            Initialize_EO0Arg = coder.types.Argument;
                    Initialize_EO0Arg.Type = Type_20_R0_V0_C0;
                    Initialize_EO0Arg.Name = 'rtY_data_frame';
                    Initialize_EO0Arg.IOType = 'OUTPUT';
                    codeInfo.Outports(0+1).Implementation.Identifier = 'rtY_data_frame';

                    tmpArgs    = [tmpArgs, Initialize_EO0Arg];
                    tmpActualArgs = [tmpActualArgs, codeInfo.Outports(0+1)];
                    wr.updateOutportArg(codeInfo.Outports(0+1), 0+1);
            
    % Matrix Type
    Type_22_R0_V0_C0 = coder.types.Matrix;
    Type_22_R0_V0_C0.BaseType   = Type_8_R0_V0_C0;
        Type_22_R0_V0_C0.Dimensions = [1848,1];
        Type_22_R0_V0_C0.SymbolicDimensions = Type_22_R0_V0_C0.SymbolicDimensions;
                            Initialize_EO1Arg = coder.types.Argument;
                    Initialize_EO1Arg.Type = Type_22_R0_V0_C0;
                    Initialize_EO1Arg.Name = 'rtY_dec_in';
                    Initialize_EO1Arg.IOType = 'OUTPUT';
                    codeInfo.Outports(1+1).Implementation.Identifier = 'rtY_dec_in';

                    tmpArgs    = [tmpArgs, Initialize_EO1Arg];
                    tmpActualArgs = [tmpActualArgs, codeInfo.Outports(1+1)];
                    wr.updateOutportArg(codeInfo.Outports(1+1), 1+1);
            
    % Matrix Type
    Type_23_R0_V0_C0 = coder.types.Matrix;
    Type_23_R0_V0_C0.BaseType   = Type_8_R0_V0_C0;
        Type_23_R0_V0_C0.Dimensions = [784,1];
        Type_23_R0_V0_C0.SymbolicDimensions = Type_23_R0_V0_C0.SymbolicDimensions;
                            Initialize_EO2Arg = coder.types.Argument;
                    Initialize_EO2Arg.Type = Type_23_R0_V0_C0;
                    Initialize_EO2Arg.Name = 'rtY_descr_in';
                    Initialize_EO2Arg.IOType = 'OUTPUT';
                    codeInfo.Outports(2+1).Implementation.Identifier = 'rtY_descr_in';

                    tmpArgs    = [tmpArgs, Initialize_EO2Arg];
                    tmpActualArgs = [tmpActualArgs, codeInfo.Outports(2+1)];
                    wr.updateOutportArg(codeInfo.Outports(2+1), 2+1);
                    Initialize_EO3Arg = coder.types.Argument;
                    Initialize_EO3Arg.Type = Type_23_R0_V0_C0;
                    Initialize_EO3Arg.Name = 'rtY_bits_out';
                    Initialize_EO3Arg.IOType = 'OUTPUT';
                    codeInfo.Outports(3+1).Implementation.Identifier = 'rtY_bits_out';

                    tmpArgs    = [tmpArgs, Initialize_EO3Arg];
                    tmpActualArgs = [tmpActualArgs, codeInfo.Outports(3+1)];
                    wr.updateOutportArg(codeInfo.Outports(3+1), 3+1);
        


        

    func_Initialize_Prototype = coder.types.Prototype;
    func_Initialize_Prototype.Arguments  = tmpArgs;
    func_Initialize_Prototype.Return     = tmpRet;
        func_Initialize_Prototype.HeaderFile = ['V2X_RX_Baseband', '.h'];
    func_Initialize_Prototype.SourceFile = ['V2X_RX_Baseband', '.c'];

    func_Initialize_Interface = RTW.FunctionInterface;
    func_Initialize_Interface.Prototype    = func_Initialize_Prototype;
    func_Initialize_Interface.ActualArgs   = tmpActualArgs;
    func_Initialize_Interface.ActualReturn = tmpActualReturn;

        func_Initialize_Prototype.Name = 'V2X_RX_Baseband_initialize';
    time_constant = codeInfo.TimingProperties(getIndexFromTimingInternalId(codeInfo.TimingInternalIds, -1));
        func_Initialize_Interface.Timing = time_constant;
        codeInfo.InitializeFunctions =  [codeInfo.InitializeFunctions, func_Initialize_Interface'];
        func_Initialize_Interface.IsInitializeFunction = true;

                  tmpArgs         = [];
    tmpActualArgs   = [];
    tmpRet          = coder.types.Argument.empty;
    tmpActualReturn = RTW.DataInterface.empty;

        
            writeCodeInfoFcn_RTArg = coder.types.Argument;
            writeCodeInfoFcn_RTArg.Type   = Ptrtype_RTModel_1_0;
            writeCodeInfoFcn_RTArg.Name   = 'rtM';
            writeCodeInfoFcn_RTArg.IOType = 'INPUT_OUTPUT';

            tmpArgs    = [tmpArgs, writeCodeInfoFcn_RTArg];
            tmpActualArgs = [tmpActualArgs, idata_RTModel];



                        OutputUpdate_EI0Arg = coder.types.Argument;
                        OutputUpdate_EI0Arg.Type = Type_19_R0_V0_C0;
                        OutputUpdate_EI0Arg.Name = 'rtU_v2x_rx_bb_in';
                        codeInfo.Inports(0+1).Implementation.Identifier = 'rtU_v2x_rx_bb_in';
                        tmpArgs    = [tmpArgs, OutputUpdate_EI0Arg];
                        tmpActualArgs = [tmpActualArgs, codeInfo.Inports(0+1)];
                        wr.updateInportArg(codeInfo.Inports(0+1), 0+1);
                    OutputUpdate_EO0Arg = coder.types.Argument;
                    OutputUpdate_EO0Arg.Type = Type_20_R0_V0_C0;
                    OutputUpdate_EO0Arg.Name = 'rtY_data_frame';
                    OutputUpdate_EO0Arg.IOType = 'OUTPUT';
                    codeInfo.Outports(0+1).Implementation.Identifier = 'rtY_data_frame';

                    tmpArgs    = [tmpArgs, OutputUpdate_EO0Arg];
                    tmpActualArgs = [tmpActualArgs, codeInfo.Outports(0+1)];
                    wr.updateOutportArg(codeInfo.Outports(0+1), 0+1);
                    OutputUpdate_EO1Arg = coder.types.Argument;
                    OutputUpdate_EO1Arg.Type = Type_22_R0_V0_C0;
                    OutputUpdate_EO1Arg.Name = 'rtY_dec_in';
                    OutputUpdate_EO1Arg.IOType = 'OUTPUT';
                    codeInfo.Outports(1+1).Implementation.Identifier = 'rtY_dec_in';

                    tmpArgs    = [tmpArgs, OutputUpdate_EO1Arg];
                    tmpActualArgs = [tmpActualArgs, codeInfo.Outports(1+1)];
                    wr.updateOutportArg(codeInfo.Outports(1+1), 1+1);
                    OutputUpdate_EO2Arg = coder.types.Argument;
                    OutputUpdate_EO2Arg.Type = Type_23_R0_V0_C0;
                    OutputUpdate_EO2Arg.Name = 'rtY_descr_in';
                    OutputUpdate_EO2Arg.IOType = 'OUTPUT';
                    codeInfo.Outports(2+1).Implementation.Identifier = 'rtY_descr_in';

                    tmpArgs    = [tmpArgs, OutputUpdate_EO2Arg];
                    tmpActualArgs = [tmpActualArgs, codeInfo.Outports(2+1)];
                    wr.updateOutportArg(codeInfo.Outports(2+1), 2+1);
                    OutputUpdate_EO3Arg = coder.types.Argument;
                    OutputUpdate_EO3Arg.Type = Type_23_R0_V0_C0;
                    OutputUpdate_EO3Arg.Name = 'rtY_bits_out';
                    OutputUpdate_EO3Arg.IOType = 'OUTPUT';
                    codeInfo.Outports(3+1).Implementation.Identifier = 'rtY_bits_out';

                    tmpArgs    = [tmpArgs, OutputUpdate_EO3Arg];
                    tmpActualArgs = [tmpActualArgs, codeInfo.Outports(3+1)];
                    wr.updateOutportArg(codeInfo.Outports(3+1), 3+1);
        


        

    func_OutputUpdate_Prototype = coder.types.Prototype;
    func_OutputUpdate_Prototype.Arguments  = tmpArgs;
    func_OutputUpdate_Prototype.Return     = tmpRet;
        func_OutputUpdate_Prototype.HeaderFile = ['V2X_RX_Baseband', '.h'];
    func_OutputUpdate_Prototype.SourceFile = ['V2X_RX_Baseband', '.c'];

    func_OutputUpdate_Interface = RTW.FunctionInterface;
    func_OutputUpdate_Interface.Prototype    = func_OutputUpdate_Prototype;
    func_OutputUpdate_Interface.ActualArgs   = tmpActualArgs;
    func_OutputUpdate_Interface.ActualReturn = tmpActualReturn;

            func_OutputUpdate_Prototype.Name = 'V2X_RX_Baseband_step';
    time_0 = codeInfo.TimingProperties(getIndexFromTimingInternalId(codeInfo.TimingInternalIds, 0));
        func_OutputUpdate_Interface.Timing = time_0;
        codeInfo.OutputFunctions =  [codeInfo.OutputFunctions, func_OutputUpdate_Interface'];

            
          



    



    



    
    

    


































    if ~isempty(skippedParameters)
    wr.removeSkippedParameters(codeInfo.Parameters(skippedParameters));
    codeInfo.Parameters(skippedParameters) = [];
    end

        codeInfo.InternalData = [codeInfo.InternalData, idata_RTModel];
      wr.addInternalData(idata_RTModel);
        codeInfo.InternalData(end+1) = idata_rt_DWork;
      wr.addInternalData(idata_rt_DWork);

    % Handling Right-Click Builds
    ss = rtwprivate('getSourceSubsystemHandle',codeInfo.GraphicalPath);
    % Check if Rt-Click build, then re-map SIDs
    if ~isempty(ss) && rtwprivate('rtwattic','hasSIDMap')
    codeInfo = modifyCodeInfoForSubsystemBuild(ss, codeInfo, wr);
    end

    wr.writeFunctionInterfaces(codeInfo);
    wr.writeServerCallPoints(codeInfo);



    wr.closeRepo();
      if exist('expInports', 'var')
      save codeInfo.mat codeInfo expInports;
      else
      save codeInfo.mat codeInfo;
      end
    returnVal = 1;

    % End Function: writeCodeInfoFcn

    function data = uniquifyDataAccess(list)
    data = RTW.DataInterface.empty;
    for idx=1:numel(list)
    if ~isPresentDI(data, list(idx))
    data(end+1) = list(idx); %#ok<AGROW>
    end
    end
    % End Function: uniquifyDataAccess

    function isPresent = isPresentDI(list, di)
    isPresent = false;
    for idx = 1:numel(list)
    if isequal(list(idx), di)
    isPresent = true;
    break
    end
    end

    function idx = getIndexFromTimingInternalId(internalIdVec, internalId)
    idx = find(internalIdVec == internalId);
    if (isempty(idx) || (length(idx) > 1))
    ciMsg = 'Time object cannot be empty';
    ciExc = MException('RTW:buildProcess:CodeInfoInternalError',ciMsg);
    throw(ciExc);
    end
    % End Function: getIndexFromTimingInternalId

    function checkDataGraphicalNames(ciName, rtwName)
    if (strcmp(ciName, rtwName) ~= 1)
    ciMsg = ['Name mismatch: ', ciName, ' and ', rtwName];
    ciExc = MException('RTW:buildProcess:CodeInfoInternalError',ciMsg);
    throw(ciExc);
    end
    % End Function: checkDataGraphicalNames

    function iData = getInternalDataByName(iDataVec, iDataName)
    iData = RTW.DataInterface.empty;
    for idxData = 1:numel(iDataVec)
    if strcmp(iDataName, iDataVec(idxData).GraphicalName)
    iData(end+1) = iDataVec(idxData);
    break
    end
    end
    if (isempty(iData) || (length(iData) > 1))
    ciMsg = 'Internal Data object cannot be empty';
    ciExc = MException('RTW:buildProcess:CodeInfoInternalError',ciMsg);
    throw(ciExc);
    end
    % End Function: getInternalDataByName

    function iData = getInternalDataByVariableName(iDataVec, iDataVariableName)
    iData = RTW.DataInterface.empty;
    for idxData = 1:numel(iDataVec)
    if strcmp(iDataVariableName, iDataVec(idxData).Implementation.VariableName)
    iData = iDataVec(idxData);
    break
    end
    end
    if isempty(iData)
    ciMsg = 'Internal Data object cannot be empty';
    ciExc = MException('RTW:buildProcess:CodeInfoInternalError',ciMsg);
    throw(ciExc);
    end
    % End Function: getInternalDataByVariableName

    function codeInfo = modifyCodeInfoForSubsystemBuild(ss, codeInfo, wr)
      % Extract Subsystem Build Map from AtticData
      Subsystem_Build_Mapping = rtwprivate('rtwattic','getSIDMap');

      % Parameters
      for i = 1:length(codeInfo.Parameters)
      sid = codeInfo.Parameters(i).SID;

      sid = Simulink.ID.getSubsystemBuildSID(sid,ss, Subsystem_Build_Mapping);
      wr.updateSIDForSubsystemBuild("Parameter", i, sid, codeInfo.Parameters(i));
      codeInfo.Parameters(i).SID = sid;
      end

      % Data Stores
      for i = 1:length(codeInfo.DataStores)
      sid = codeInfo.DataStores(i).SID;

      sid = Simulink.ID.getSubsystemBuildSID(sid,ss, Subsystem_Build_Mapping);
      codeInfo.DataStores(i).SID = sid;
      wr.updateSIDForSubsystemBuild("DataStore", i, sid);

      end

      % Inports
      for i = 1:length(codeInfo.Inports)
      sid = codeInfo.Inports(i).SID;

      sid = Simulink.ID.getSubsystemBuildSID(sid,ss, Subsystem_Build_Mapping);
      codeInfo.Inports(i).SID = sid;
      wr.updateSIDForSubsystemBuild("Inport", i, sid);

      end

      % Outports
      for i = 1:length(codeInfo.Outports)
      sid = codeInfo.Outports(i).SID;

      sid = Simulink.ID.getSubsystemBuildSID(sid,ss, Subsystem_Build_Mapping);
      codeInfo.Outports(i).SID = sid;
      wr.updateSIDForSubsystemBuild("Outport", i, sid);

      end

