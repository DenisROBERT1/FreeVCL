object Form_DisplayError: TForm_DisplayError
  Left = 100
  Top = 100
  Width = 520
  Height = 200
  Caption = ''
  BorderIcons = [biSystemMenu]
  Position = poOwnerFormCenter
  Constraints.MinWidth = 320
  Constraints.MinHeight = 160
  OnClose = FormClose
  object Image: TImage
    Left = 4
    Top = 4
    Width = 48
    Height = 48
    Anchors = [akLeft, akTop]
  end
  object Label_Message: TLabel
    Left = 64
    Top = 4
    Width = 448
    Height = 100
    Anchors = [akLeft, akTop, akRight, akBottom]
    Caption = ''
    WordWrap = True
  end
  object CheckBox_NePlusAfficher: TCheckBox
    Left = 4
    Top = 108
    Width = 456
    Height = 13
    Anchors = [akLeft, akBottom]
    Alignment = taRightJustify
    Caption = 'Ne plus afficher ce message'
  end
  object Button_Oui: TButton
    Left = 224
    Top = 136
    Width = 68
    Height = 23
    Anchors = [akLeft, akBottom]
    Caption = 'Oui'
    TabStop = True
    TabOrder = 0
    OnClick = Button_OuiClick
  end
  object Button_Non: TButton
    Left = 224
    Top = 136
    Width = 68
    Height = 23
    Anchors = [akLeft, akBottom]
    Caption = 'Non'
    TabStop = True
    TabOrder = 1
    OnClick = Button_NonClick
  end
  object Button_Ok: TButton
    Left = 224
    Top = 136
    Width = 68
    Height = 23
    Anchors = [akLeft, akBottom]
    Caption = 'Ok'
    TabStop = True
    TabOrder = 2
    Default = True
    OnClick = Button_OkClick
  end
  object Button_Annuler: TButton
    Left = 224
    Top = 136
    Width = 68
    Height = 23
    Anchors = [akLeft, akBottom]
    Caption = 'Annuler'
    TabStop = True
    TabOrder = 3
    OnClick = Button_AnnulerClick
  end
  object Button_Aide: TButton
    Left = 224
    Top = 136
    Width = 68
    Height = 23
    Anchors = [akLeft, akBottom]
    Caption = 'Aide'
    TabStop = True
    TabOrder = 4
    OnClick = Button_AideClick
  end
end
