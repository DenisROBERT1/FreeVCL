object Form_EditProject: TForm_EditProject
  Caption = 'Modification d'#39'une application FreeVCL'
  Left = 4
  Top = 0
  Width = 518
  Height = 274
  Constraints.MinWidth = 534
  Constraints.MinHeight = 296
  Position = poOwnerFormCenter
  object PageControl: TPageControl
    Caption = 'TPageControl'
    Left = 4
    Top = 4
    Width = 500
    Height = 188
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabStop = True
    ActivePage = TTabSheet_Fichiers
    ActivePageIndex = 0
    object TabSheet_OptionsGenerales: TTabSheet
      Caption = 'Options générales'
      object Label_Compilateur: TLabel
        Caption = 'Compilateur'
        Left = 8
        Top = 6
        Width = 78
        Height = 13
      end
      object ComboBox_Compilateur: TComboBox
        Left = 96
        Top = 4
        Width = 148
        Height = 69
        Style = csDropDownList
      end
      object Label_Output: TLabel
        Caption = 'Fichier de sortie'
        Left = 8
        Top = 28
        Width = 78
        Height = 13
      end
      object Edit_Output: TEdit
        Left = 96
        Top = 26
        Width = 328
        Height = 21
		    Anchors = [akLeft, akTop, akRight]
      end
      object Button_ParcourirOutput: TButton
        Caption = '&Parcourir'
        Left = 428
        Top = 26
        Width = 60
        Height = 21
		    Anchors = [akTop, akRight]
				OnClick = Button_ParcourirOutputClick
      end
			object SaveDialog_Output: TSaveDialog
				Title = 'Fichier exécutable à créer'
				Filter = 'Programmes (*.exe)|*.exe|Tous (*.*)|*.*|'
				DefaultExt = 'exe'
				Options = [ofHideReadOnly, ofEnableSizing]
			end
    end
    object TabSheet_OptionsCompilation: TTabSheet
      Caption = 'Options de compilation'
    end
    object TabSheet_Fichiers: TTabSheet
      Caption = 'Fichiers du projet'
      Anchors = [akLeft, akTop, akRight, akBottom]
      object TreeView_SourceFiles: TTreeView
        Left = 4
        Top = 2
        Width = 420
        Height = 158
        Anchors = [akLeft, akTop, akRight, akBottom]
        TabStop = True
        TabOrder = 0
				OnChange = TreeView_SourceFilesChange
      end
			object Button_Ajouter: TButton
				Caption = '&Ajouter'
				Left = 428
				Top = 2
				Width = 60
				Height = 20
				Anchors = [akTop, akRight]
				TabOrder = 1
				OnClick = Button_AjouterClick
			end
			object Button_Enlever: TButton
				Caption = '&Enlever'
				Left = 428
				Top = 24
				Width = 60
				Height = 20
				Anchors = [akTop, akRight]
				TabOrder = 2
				Enabled = False
				OnClick = Button_EnleverClick
			end
			object OpenDialog_AddFile: TOpenDialog
				Left = 400
				Top = 32
				Filter = 'Programmes (*.c;*.cpp;*.h)|*.c;*.cpp;*.h|Ressources (*.rc;*.dfm)|*.rc;*.dfm|Tous (*.*)|*.*'
				Options = [ofFileMustExist, ofEnableSizing, ofAllowMultiSelect]
			end
    end
  end
  object Button_Ok: TButton
    Caption = 'Ok'
    Left = 150
    Top = 211
    Width = 60
    Height = 20
    Anchors = [akLeft, akBottom]
    TabStop = True
    TabOrder = 1
    Default = True
    ModalResult = mrOk
  end
  object Button_Annuler: TButton
    Caption = 'Annuler'
    Left = 225
    Top = 211
    Width = 60
    Height = 20
    Anchors = [akLeft, akBottom]
    TabStop = True
    TabOrder = 2
    ModalResult = mrCancel
  end
  object Button_Aide: TButton
    Caption = 'Aide'
    Left = 300
    Top = 211
    Width = 60
    Height = 20
    Anchors = [akLeft, akBottom]
    OnClick = Button_AideClick
    TabStop = True
    TabOrder = 3
  end
end

