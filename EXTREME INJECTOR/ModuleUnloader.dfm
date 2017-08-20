object ModuleUnloaderClass: TModuleUnloaderClass
  Left = 405
  Top = 260
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsToolWindow
  Caption = '                '#1052#1086#1076#1091#1083#1100' '#1056#1072#1079#1075#1088#1091#1079#1095#1080#1082
  ClientHeight = 236
  ClientWidth = 265
  Color = clGray
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object cbModules: TGroupBox
    Left = 8
    Top = 2
    Width = 249
    Height = 227
    Caption = #1052#1086#1076#1091#1083#1080
    TabOrder = 0
    object lbLoadedModules: TLabel
      Left = 19
      Top = 203
      Width = 138
      Height = 13
      Caption = #1092#1091#1085#1090' '#1047#1072#1075#1088#1091#1078#1077#1085#1085#1099#1077' '#1084#1086#1076#1091#1083#1080
    end
    object ListView1: TListView
      Left = 19
      Top = 47
      Width = 214
      Height = 150
      Columns = <
        item
          Caption = #1048#1084#1103' '#1084#1086#1076#1091#1083#1103
          Width = 120
        end
        item
          Caption = #1041#1072#1079#1072' Addr'
          Width = 70
        end>
      ReadOnly = True
      RowSelect = True
      TabOrder = 0
      ViewStyle = vsReport
      OnClick = ListView1Click
    end
    object btnLoadModules: TButton
      Left = 19
      Top = 20
      Width = 102
      Height = 21
      Caption = #1047#1072#1075#1088#1091#1079#1082#1072' '#1084#1086#1076#1091#1083#1077#1081
      TabOrder = 1
      OnClick = btnLoadModulesClick
    end
    object btnUnloadModule: TButton
      Left = 127
      Top = 20
      Width = 98
      Height = 21
      Caption = #1042#1099#1075#1088#1091#1079#1082#1072' '#1084#1086#1076#1091#1083#1103
      TabOrder = 2
      OnClick = btnUnloadModuleClick
    end
  end
end
