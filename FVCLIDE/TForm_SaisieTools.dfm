object Form_SaisieTools: TForm_SaisieTools
  Caption = 'Outil utilisateur'
  Left = 452
  Top = 230
  Width = 400
  Height = 186
  Constraints.MinWidth = 400
  Constraints.MinHeight = 186
  Constraints.MaxHeight = 186
  Position = poOwnerFormCenter
  object Label_Name: TLabel
    Caption = 'Nom :'
    Left = 8
    Top = 10
    Width = 68
    Height = 14
    Anchors = [akLeft, akTop]
  end
  object Edit_Name: TEdit
    Left = 80
    Top = 6
    Width = 236
    Height = 18
    Anchors = [akLeft, akTop, akRight]
    TabStop = True
    TabOrder = 0
  end
  object Label_Prog: TLabel
    Caption = 'Programme :'
    Left = 8
    Top = 34
    Width = 68
    Height = 14
    Anchors = [akLeft, akTop]
  end
  object Edit_Prog: TEdit
    Left = 80
    Top = 32
    Width = 236
    Height = 18
    Anchors = [akLeft, akTop, akRight]
    TabStop = True
    TabOrder = 1
    OnExit = Edit_ProgExit
  end
  object Button_BrowseProg: TButton
    Caption = 'Parcourir ...'
    Left = 320
    Top = 32
    Width = 68
    Height = 18
    Anchors = [akTop, akRight]
    TabStop = False
		OnClick = Button_BrowseProgClick
  end
  object OpenDialog_Prog: TOpenDialog
    Title = 'Programme à lancer'
    Filter = 'Programmes (*.exe)|*.exe|Tous (*.*)|*.*|'
    DefaultExt = 'exe'
    Options = [ofHideReadOnly, ofFileMustExist]
  end
  object Label_Args: TLabel
    Caption = 'Arguments :'
    Left = 8
    Top = 60
    Width = 68
    Height = 14
    Anchors = [akLeft, akTop]
  end
  object Edit_Args: TEdit
    Left = 80
    Top = 58
    Width = 236
    Height = 18
    Anchors = [akLeft, akTop, akRight]
    TabStop = True
    TabOrder = 2
    OnExit = Edit_ArgsExit
  end
  object Button_BrowseArgs: TButton
    Caption = 'Parcourir ...'
    Left = 320
    Top = 58
    Width = 68
    Height = 18
    Anchors = [akTop, akRight]
    TabStop = False
		OnClick = Button_BrowseArgsClick
  end
  object OpenDialog_Args: TOpenDialog
    Title = 'Argument'
    Filter = 'Tous (*.*)|*.*|'
    Options = [ofHideReadOnly, ofFileMustExist]
  end
  object Label_InitDir: TLabel
    Caption = 'Répertoire :'
    Left = 8
    Top = 86
    Width = 68
    Height = 14
    Anchors = [akLeft, akTop]
  end
  object Edit_InitDir: TEdit
    Left = 80
    Top = 84
    Width = 236
    Height = 18
    Anchors = [akLeft, akTop, akRight]
    TabStop = True
    TabOrder = 3
  end
  object Button_BrowseDir: TButton
    Caption = 'Parcourir ...'
    Left = 320
    Top = 84
    Width = 68
    Height = 18
    Anchors = [akTop, akRight]
    TabStop = False
		OnClick = Button_BrowseDirClick
  end
  object Button_Ok: TButton
    Caption = 'Ok'
    Left = 74
    Top = 122
    Width = 68
    Height = 23
    Anchors = [akLeft, akTop]
    TabStop = True
    TabOrder = 4
    Default = True
    OnClick = Button_OkClick
  end
  object Button_Annuler: TButton
    Caption = 'Annuler'
    Left = 162
    Top = 122
    Width = 68
    Height = 23
    Anchors = [akLeft, akTop]
    TabStop = True
    TabOrder = 5
    Cancel = True
    ModalResult = mrCancel
  end
  object Button_Aide: TButton
    Caption = 'Aide'
    Left = 250
    Top = 122
    Width = 68
    Height = 23
    Anchors = [akLeft, akTop]
    TabStop = True
    TabOrder = 6
    OnClick = Button_AideClick
  end
end
