import tkinter as tk
from tkinter import filedialog, messagebox, scrolledtext
from text_ui import TextConverter
from pathlib import Path
import os

class TextConverterGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("文本转换系统")
        self.base_dir = Path(__file__).parent
        self.cpp_executable = self.base_dir / "cpp_core" / "text_transform"
        if os.name == 'nt':
            self.cpp_executable = self.cpp_executable.with_suffix(".exe")
        self.converter = TextConverter(self.cpp_executable)
        self.create_widgets()

    def create_widgets(self):
        # 规则文件选择
        tk.Label(self.root, text="选择规则文件:").grid(row=0, column=0, sticky="w")
        self.rule_var = tk.StringVar(value="default_rules.txt")
        self.rule_menu = tk.OptionMenu(self.root, self.rule_var, *self.converter.list_rule_files())
        self.rule_menu.grid(row=0, column=1, sticky="ew")

        # 文本输入
        tk.Label(self.root, text="输入文本:").grid(row=1, column=0, sticky="nw")
        self.text_input = scrolledtext.ScrolledText(self.root, width=50, height=8)
        self.text_input.grid(row=1, column=1, padx=5, pady=5)

        # 转换按钮
        self.convert_btn = tk.Button(self.root, text="文本转换", command=self.convert_text)
        self.convert_btn.grid(row=2, column=1, sticky="e", pady=5)

        # 文件转换
        tk.Label(self.root, text="文件输入转换:").grid(row=3, column=0, sticky="w")
        self.file_var = tk.StringVar()
        self.file_menu = tk.OptionMenu(self.root, self.file_var, *self.converter.list_input_files())
        self.file_menu.grid(row=3, column=1, sticky="ew")
        self.file_convert_btn = tk.Button(self.root, text="文件转换", command=self.convert_file)
        self.file_convert_btn.grid(row=4, column=1, sticky="e", pady=5)

        # 结果显示
        tk.Label(self.root, text="转换结果:").grid(row=5, column=0, sticky="nw")
        self.result_box = scrolledtext.ScrolledText(self.root, width=50, height=10)
        self.result_box.grid(row=5, column=1, padx=5, pady=5)

    def convert_text(self):
        rules_file = self.rule_var.get()
        input_text = self.text_input.get("1.0", tk.END).strip()
        if not input_text:
            messagebox.showwarning("提示", "输入文本不能为空！")
            return
        success, result = self.converter.run_conversion(
            rules_file=self.converter.rules_dir / rules_file,
            input_text=input_text
        )
        self.result_box.delete("1.0", tk.END)
        self.result_box.insert(tk.END, result if success else f"错误: {result}")

    def convert_file(self):
        rules_file = self.rule_var.get()
        input_file = self.file_var.get()
        if not input_file:
            messagebox.showwarning("提示", "请选择输入文件！")
            return
        input_path = self.converter.inputs_dir / input_file
        success, result = self.converter.run_conversion(
            rules_file=self.converter.rules_dir / rules_file,
            input_text="",
            input_file=input_path
        )
        self.result_box.delete("1.0", tk.END)
        self.result_box.insert(tk.END, result if success else f"错误: {result}")

if __name__ == "__main__":
    root = tk.Tk()
    app = TextConverterGUI(root)
    root.mainloop()