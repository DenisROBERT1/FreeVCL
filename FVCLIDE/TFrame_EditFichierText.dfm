object Frame_EditFichierText: TFrame_EditFichierText
  Left = 282
  Top = 133
  Width = 460
  Height = 383
  Color = clBtnFace
  Visible = True
  OnShow = Frame_EditFichierTextShow
  object Panel: TPanel
    Left = 6
    Top = 6
    Width = 439
    Height = 325
    Align = alClient
    TabStop = True
    TabOrder = 0
    BevelOuter = bvLowered
    object TextEditor: TTextEditor
      Left = 0
      Top = 0
      Width = 439
      Height = 325
      Align = alClient
      TabStop = True
      TabOrder = 0
      DragAcceptFiles = True
      PopupMenu = PopupMenu_Context
	  ShowHint = False
	  Hint = ''
      OnContextPopup = TextEditorContextPopup
      OnDropFile = TextEditorDropFile
    end
  end
  object PopupMenu_Context: TPopupMenu
  end
end
