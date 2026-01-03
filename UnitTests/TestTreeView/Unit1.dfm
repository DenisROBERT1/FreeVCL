object Unit1: TUnit1
  Left = 311
  Top = 180
  Width = 350
  Height = 458
  Caption = 'Test TreeView'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object TreeView1: TTreeView
    Left = 6
    Top = 12
    Width = 241
    Height = 409
    ShowRoot = True
    ShowButtons = True
    HideSelection = False
    Indent = 19
    TabOrder = 0
  end
  object Edit1: TEdit
    Left = 258
    Top = 12
    Width = 73
    Height = 21
    TabOrder = 1
  end
  object Button_AddRoot: TButton
    Left = 258
    Top = 40
    Width = 73
    Height = 21
    Caption = 'Add root'
    TabOrder = 2
    OnClick = Button_AddRootClick
  end
  object Button_AddChild: TButton
    Left = 258
    Top = 96
    Width = 73
    Height = 21
    Caption = 'Add child'
    TabOrder = 3
    OnClick = Button_AddChildClick
  end
  object Button_Add: TButton
    Left = 258
    Top = 68
    Width = 73
    Height = 21
    Caption = 'Add'
    TabOrder = 4
    OnClick = Button_AddClick
  end
  object Button_Delete: TButton
    Left = 258
    Top = 124
    Width = 73
    Height = 21
    Caption = 'Delete'
    TabOrder = 5
    OnClick = Button_DeleteClick
  end
end
