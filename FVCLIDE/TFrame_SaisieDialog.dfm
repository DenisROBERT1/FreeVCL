object Frame_SaisieDialog: TFrame_SaisieDialog
  Left = 282
  Top = 133
  Width = 460
  Height = 383
  Color = clBtnFace
  Visible = True
  OnShow = Frame_SaisieDialogShow
  OnHide = Frame_SaisieDialogHide
  object ImageList_Popup: TImageList
    Width = 24
    Height = 24
    Masked = False
    ResourceName = 'BMP_IMAGELISTPOPUP'
  end
  object PopupMenu_ModifObjet: TPopupMenu
    AutoPopup = False
    object MenuItem_Aspect: TMenuItem
      Caption = 'Aspect'
      object MenuItem_AlignGrille: TMenuItem
        Caption = 'Aligner sur la grille'
        OnClick = MenuItem_AlignGrilleClick
      end
      object MenuItem_Unif: TMenuItem
        Caption = 'Uniformiser'
        object MenuItem_UnifWidth: TMenuItem
          Caption = 'La largeur'
          OnClick = MenuItem_UnifWidthClick
        end
        object MenuItem_UnifHeight: TMenuItem
          Caption = 'La hauteur'
          OnClick = MenuItem_UnifHeightClick
        end
        object MenuItem_UnifSize: TMenuItem
          Caption = 'La taille (largeur + hauteur)'
          OnClick = MenuItem_UnifSizeClick
        end
      end
      object MenuItem_AlignDep: TMenuItem
        Caption = 'Aligner en déplaçant'
        object MenuItem_AlignDepLeft: TMenuItem
          SubMenuImages = ImageList_Popup
          ImageIndex = 0
          Caption = 'Sur le bord gauche'
          OnClick = MenuItem_AlignDepLeftClick
        end
        object MenuItem_AlignDepCenterH: TMenuItem
          SubMenuImages = ImageList_Popup
          ImageIndex = 1
          Caption = 'Centrer horizontalement'
          OnClick = MenuItem_AlignDepCenterHClick
        end
        object MenuItem_AlignDepRight: TMenuItem
          SubMenuImages = ImageList_Popup
          ImageIndex = 2
          Caption = 'Sur le bord droit'
          OnClick = MenuItem_AlignDepRightClick
        end
        object MenuItem_AlignDepTop: TMenuItem
          SubMenuImages = ImageList_Popup
          ImageIndex = 3
          Caption = 'Sur le bord haut'
          OnClick = MenuItem_AlignDepTopClick
        end
        object MenuItem_AlignDepCenterV: TMenuItem
          SubMenuImages = ImageList_Popup
          ImageIndex = 4
          Caption = 'Centrer verticalement'
          OnClick = MenuItem_AlignDepCenterVClick
        end
        object MenuItem_AlignDepBottom: TMenuItem
          SubMenuImages = ImageList_Popup
          ImageIndex = 5
          Caption = 'Sur le bord bas'
          OnClick = MenuItem_AlignDepBottomClick
        end
      end
      object MenuItem_AlignDim: TMenuItem
        Caption = 'Aligner en redimensionnant'
        object MenuItem_AlignDimLeft: TMenuItem
          SubMenuImages = ImageList_Popup
          ImageIndex = 0
          Caption = 'Sur le bord gauche'
          OnClick = MenuItem_AlignDimLeftClick
        end
        object MenuItem_AlignDimRight: TMenuItem
          SubMenuImages = ImageList_Popup
          ImageIndex = 2
          Caption = 'Sur le bord droit'
          OnClick = MenuItem_AlignDimRightClick
        end
        object MenuItem_AlignDimTop: TMenuItem
          SubMenuImages = ImageList_Popup
          ImageIndex = 3
          Caption = 'Sur le bord haut'
          OnClick = MenuItem_AlignDimTopClick
        end
        object MenuItem_AlignDimBottom: TMenuItem
          SubMenuImages = ImageList_Popup
          ImageIndex = 5
          Caption = 'Sur le bord bas'
          OnClick = MenuItem_AlignDimBottomClick
        end
      end
      object MenuItem_AlignButtons: TMenuItem
        Caption = 'Aligner les boutons'
        object MenuItem_AlignButtonsBottom: TMenuItem
          Caption = 'En bas'
          OnClick = MenuItem_AlignButtonsBottomClick
        end
        object MenuItem_AlignButtonsRight: TMenuItem
          Caption = 'À droite'
          OnClick = MenuItem_AlignButtonsRightClick
        end
      end
    end
    object MenuItem_Sep1: TMenuItem
      Caption = '-'
    end
    object MenuItem_Couper: TMenuItem
      Caption = 'Couper'
    end
    object MenuItem_Copier: TMenuItem
      Caption = 'Copier'
    end
    object MenuItem_Coller: TMenuItem
      Caption = 'Coller'
    end
    object MenuItem_Sep2: TMenuItem
      Caption = '-'
    end
    object MenuItem_Supprimer: TMenuItem
      Caption = 'Supprimer'
      OnClick = MenuItem_SupprimerClick
    end
    object MenuItem_Sep3: TMenuItem
      Caption = '-'
    end
    object MenuItem_MettreAvant: TMenuItem
      Caption = 'Mettre avant'
      OnClick = MenuItem_MettreAvantClick
    end
    object MenuItem_MettreApres: TMenuItem
      Caption = 'Mettre après'
      OnClick = MenuItem_MettreApresClick
    end
    object MenuItem_MettreEnfant: TMenuItem
      Caption = 'Mettre comme enfant'
      OnClick = MenuItem_MettreEnfantClick
    end
    object MenuItem_MettreFrere: TMenuItem
      Caption = 'Mettre comme frère'
      OnClick = MenuItem_MettreFrereClick
    end
    object MenuItem_Sep4: TMenuItem
      Caption = '-'
    end
    object MenuItem_Proprietes: TMenuItem
      Caption = 'Propriétés'
      OnClick = MenuItem_ProprietesClick
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 140
    Height = 300
    Align = alLeft
    BorderStyle = bsNone
    BevelOuter = bvNone
    object Label_Dialogues1: TLabel
      Left = 6
      Top = 4
      Width = 53
      Height = 13
      Caption = 'Dialogues :'
    end
    object TreeView_Forms: TTreeView
      Left = 6
      Top = 24
      Width = 126
      Height = 316
      Anchors = [akLeft, akTop, akRight, akBottom]
      ItemHeight = 13
      TabStop = True
      TabOrder = 0
      AutoPopup = True
      MultiSelect = True
      MultiSelectStyle = msControlSelect
      RightClickSelect = True
      DragMode = dmAutomatic
      OnContextPopup = TreeView_FormsContextPopup
      OnChange = TreeView_FormsChange
      OnDblClick = TreeView_FormsDblClick
      OnDragOver = TreeView_FormsDragOver
			OnEndDrag = TreeView_FormsEndDrag
			OnEdited = TreeView_FormsEdited
    end
    object Label_Dialogues2: TLabel
      Left = 6
      Top = 344
      Width = 60
      Height = 13
      AutoSize = False
      Anchors = [akLeft, akBottom]
      Caption = 'Dialogues'
    end
    object Label_NbDialogues: TLabel
      Left = 66
      Top = 344
      Width = 60
      Height = 13
      AutoSize = False
      Anchors = [akLeft, akBottom]
      Caption = '0'
    end
    object Label_Controles: TLabel
      Left = 6
      Top = 362
      Width = 60
      Height = 13
      AutoSize = False
      Anchors = [akLeft, akBottom]
      Caption = 'Contr'#244'les'
    end
    object Label_NbControles: TLabel
      Left = 66
      Top = 362
      Width = 60
      Height = 13
      AutoSize = False
      Anchors = [akLeft, akBottom]
      Caption = '0'
    end
  end
  object Splitter: TSplitter
    Left = 140
    Top = 0
    Width = 5
    Height = 300
    Cursor = crHSplit
    Align = alLeft
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 140
    Height = 300
    Align = alClient
    BorderStyle = bsNone
    BevelOuter = bvNone
    object Label_ObjToCreate: TLabel
      Left = 8
      Top = 6
      Width = 80
      Height = 13
      Anchors = [akLeft, akTop]
      Caption = 'Objet à créer :'
    end
    object Label_ObjName: TLabel
      Left = 92
      Top = 6
      Width = 200
      Height = 13
      Anchors = [akLeft, akTop, akRight]
      Caption = ''
    end
    object PageControl_ObjToCreate: TPageControl
      Left = 8
      Top = 24
      Width = 300
      Height = 82
      Anchors = [akLeft, akTop, akRight]
      TabStop = True
      TabOrder = 0
    end
    object BitBtn_Select: TBitBtn
      Left = 8
      Top = 110
      Width = 38
      Height = 38
      Anchors = [akLeft, akTop]
      TabStop = True
      TabOrder = 1
      ShowHint = True
      Hint = 'Mode selection'
      OnClick = BitBtn_SelectClick
      Glyph.Data = {
				76020000424D7602000000000000760000002800000020000000200000000100
				0400000000000002000000000000000000000000000000000000000000000000
				8000008000000080800080000000800080008080000080808000C0C0C0000000
				FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
				8888888888888888888888888888888888888888888888888888888888888888
				8888888888888888888888888888888888888888888888888888888888888888
				8888888888888888888888888888888888888888888888888888888888888888
				8888888888888888888888888888888888888888888888888888888888888888
				8888800888888888888888888888880888880FF0888888888888888888888800
				88880FF088888888888888888888880F0880FF0888888888888888888888880F
				F080FF0888888888888888888888880FFF0FF08888888888888888888888880F
				FFFFF00000888888888888888888880FFFFFFFFF08888888888888888888880F
				FFFFFFF088888888888888888888880FFFFFFF0888888888888888888888880F
				FFFFF08888888888888888888888880FFFFF088888888888888888888888880F
				FFF0888888888888888888888888880FFF08888888888888888888888888880F
				F088888888888888888888888888880F08888888888888888888888888888800
				8888888888888888888888888888880888888888888888888888888888888888
				8888888888888888888888888888888888888888888888888888888888888888
				8888888888888888888888888888888888888888888888888888888888888888
				8888888888888888888888888888888888888888888888888888}
    end
    object GroupBox_Grid: TGroupBox
      Left = 58
      Top = 108
      Width = 160
      Height = 56
      Caption = 'Grille'
      TabStop = True
      TabOrder = 2
			object CheckBox_Aligner: TCheckBox
				Left = 8
				Top = 12
				Width = 68
				Height = 17
				Caption = 'Aligner'
				TabStop = True
				TabOrder = 0
				OnClick = CheckBox_AlignerClick
			end
			object CheckBox_Afficher: TCheckBox
				Left = 8
				Top = 32
				Width = 68
				Height = 17
				Caption = 'Afficher'
				TabStop = True
				TabOrder = 1
				OnClick = CheckBox_AfficherClick
			end
			object Label_GridSizeX: TLabel
				Left = 80
				Top = 12
				Width = 16
				Height = 17
				Caption = 'X:'
			end
			object Edit_GridSizeX: TEdit
				Left = 100
				Top = 10
				Width = 32
				Height = 21
				Enabled = False
				TabStop = True
				TabOrder = 2
				Text = '4'
				OnChange = Edit_GridSizeXChange
			end
			object UpDown_GridSizeX: TUpDown
				Left = 132
				Top = 10
				Width = 16
				Height = 21
				Associate = Edit_GridSizeX
				Enabled = False
				Min = 0
				Position = 4
				Increment = 1
			end
			object Label_GridSizeY: TLabel
				Left = 80
				Top = 32
				Width = 16
				Height = 17
				Caption = 'Y:'
			end
			object Edit_GridSizeY: TEdit
				Left = 100
				Top = 30
				Width = 32
				Height = 21
				Enabled = False
				TabStop = True
				TabOrder = 3
				Text = '4'
				OnChange = Edit_GridSizeYChange
			end
			object UpDown_GridSizeY: TUpDown
				Left = 132
				Top = 30
				Width = 16
				Height = 21
				Associate = Edit_GridSizeY
				Enabled = False
				Min = 0
				Position = 4
				Increment = 1
			end
		end
    object CheckBox_OrdreTab: TCheckBox
      Left = 246
      Top = 120
      Width = 68
      Height = 17
      Caption = 'TabOrder'
      TabStop = True
      TabOrder = 3
      OnClick = CheckBox_OrdreTabClick
    end
		object Button_Renumeroter: TButton
			Caption = 'Renuméroter'
			Left = 328
			Top = 118
			Width = 68
			Height = 22
      TabStop = True
			TabOrder = 4
			ShowHint = True
			Hint = 'Renuméroter automatiquement les TabOrder'
      OnClick = Button_RenumeroterClick
		end
		object Button_MettreAvant: TButton
			Caption = 'Avant'
			Left = 8
			Top = 168
			Width = 68
			Height = 22
      TabStop = True
			TabOrder = 5
			ShowHint = True
			Hint = 'Déplacer avant le contrôle précédent'
      OnClick = MenuItem_MettreAvantClick
		end
		object Button_MettreApres: TButton
			Caption = 'Après'
			Left = 8
			Top = 192
			Width = 68
			Height = 22
      TabStop = True
			TabOrder = 6
			ShowHint = True
			Hint = 'Déplacer après le contrôle suivant'
      OnClick = MenuItem_MettreApresClick
		end
		object Button_MettreEnfant: TButton
			Caption = 'Enfant'
			Left = 80
			Top = 168
			Width = 68
			Height = 22
      TabStop = True
			TabOrder = 7
			ShowHint = True
			Hint = 'Déplacer comme enfant du contrôle précédent'
      OnClick = MenuItem_MettreEnfantClick
		end
		object Button_MettreFrere: TButton
			Caption = 'Frère'
			Left = 80
			Top = 192
			Width = 68
			Height = 22
      TabStop = True
			TabOrder = 8
			ShowHint = True
			Hint = 'Déplacer comme frère du contrôle parent'
      OnClick = MenuItem_MettreFrereClick
		end
		object Button_Aide: TButton
			Caption = 'Aide'
			Left = 8
			Top = 220
			Width = 68
			Height = 22
      TabStop = True
			TabOrder = 9
			OnClick = Button_AideClick
		end
  end
end
