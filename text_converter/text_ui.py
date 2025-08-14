import os
import subprocess
import shutil
import tempfile
from pathlib import Path

class TextConverter:
    def __init__(self, cpp_executable):
        self.cpp_executable = Path(cpp_executable).resolve()
        self.rules_dir = Path("rules").resolve()
        self.inputs_dir = Path("inputs").resolve()
        self.outputs_dir = Path("outputs").resolve()
        
        # 创建必要目录
        self.rules_dir.mkdir(exist_ok=True)
        self.inputs_dir.mkdir(exist_ok=True)
        self.outputs_dir.mkdir(parents=True, exist_ok=True)
        
        # 设置默认规则文件
        self.default_rules = self.rules_dir / "default_rules.txt"
        if not self.default_rules.exists():
            with open(self.default_rules, "w") as f:
                f.write("# 默认转换规则\n")
                f.write("hello hi\n")
                f.write("world earth\n")
    
    def run_conversion(self, rules_file, input_text, input_file=None, output_file=None):
        """运行转换过程"""
        # 创建临时目录
        with tempfile.TemporaryDirectory() as tmpdir:
            tmp_path = Path(tmpdir)
            
            # 处理输入
            if input_file:
                input_path = Path(input_file)
                if not input_path.exists():
                    return False, f"输入文件不存在: {input_file}"
                
                # 复制到临时目录
                temp_input = tmp_path / "input.txt"
                shutil.copy(input_path, temp_input)
            else:
                # 从文本创建输入文件
                temp_input = tmp_path / "input.txt"
                with open(temp_input, "w") as f:
                    f.write(input_text)
            
            # 处理规则
            rules_path = Path(rules_file)
            if not rules_path.exists():
                return False, f"规则文件不存在: {rules_file}"
            
            # 处理输出
            if output_file:
                output_path = Path(output_file)
            else:
                # 生成输出文件名
                output_name = "output_" + temp_input.stem + ".txt"
                output_path = self.outputs_dir / output_name
            
            # 确保输出目录存在
            output_path.parent.mkdir(parents=True, exist_ok=True)
            
            # 运行C++程序
            try:
                result = subprocess.run(
                    [str(self.cpp_executable), 
                     str(rules_path), 
                     str(temp_input), 
                     str(output_path)],
                    capture_output=True,
                    text=True,
                    check=True,
                    encoding="utf-8"  
                )
                
                # 检查输出文件是否创建
                if not output_path.exists():
                    # 尝试从标准输出获取结果
                    if result.stdout:
                        return True, result.stdout
                    else:
                        return False, "转换成功但未创建输出文件"
                
                # 读取输出文件内容
                with open(output_path, "r") as f:
                    output_content = f.read()
                
                return True, output_content
            except subprocess.CalledProcessError as e:
                # 捕获C++程序的错误输出
                error_msg = e.stderr if e.stderr else "未知错误"
                return False, f"转换失败: {error_msg}"

    def list_rule_files(self):
        """列出所有规则文件"""
        return [f.name for f in self.rules_dir.glob("*.txt")]
    
    def create_rule_file(self, filename, content):
        """创建新的规则文件"""
        filepath = self.rules_dir / filename
        if not filepath.suffix:
            filepath = filepath.with_suffix(".txt")
        
        if filepath.exists():
            return False, f"文件已存在: {filepath.name}"
        
        with open(filepath, "w") as f:
            f.write(content)
        
        return True, f"规则文件已创建: {filepath.name}"
    
    def edit_rule_file(self, filename, content):
        """编辑规则文件"""
        filepath = self.rules_dir / filename
        if not filepath.exists():
            return False, f"文件不存在: {filepath.name}"
        
        with open(filepath, "w") as f:
            f.write(content)
        
        return True, f"规则文件已更新: {filepath.name}"
    
    def get_rule_file_content(self, filename):
        """获取规则文件内容"""
        filepath = self.rules_dir / filename
        if not filepath.exists():
            return None
        
        with open(filepath, "r") as f:
            return f.read()
    
    def list_input_files(self):
        """列出所有输入文件"""
        return [f.name for f in self.inputs_dir.glob("*")]
    
    def list_output_files(self):
        """列出所有输出文件"""
        return [f.name for f in self.outputs_dir.glob("*")]

def main():
    # 设置路径
    base_dir = Path(__file__).parent
    cpp_executable = base_dir / "cpp_core" / "text_transform"
    
    # Windows系统处理
    if os.name == 'nt':
        cpp_executable = cpp_executable.with_suffix(".exe")
    
    # 检查C++程序是否存在
    if not cpp_executable.exists():
        print(f"错误: C++核心程序未找到! 请先编译: {cpp_executable}")
        print("1. 进入 cpp_core 目录")
        print("2. 运行 'make' (Linux/macOS) 或 'g++ -std=c++17 -o text_transform text_transform.cpp' (Windows)")
        return
    
    converter = TextConverter(cpp_executable)
    
    print("文本转换系统 (C++核心 + Python界面)")
    print("=" * 50)
    
    while True:
        print("\n主菜单:")
        print("1. 使用文本输入进行转换")
        print("2. 使用文件输入进行转换")
        print("3. 管理规则文件")
        print("4. 查看输入文件列表")
        print("5. 查看输出文件列表")
        print("6. 退出")
        
        choice = input("请选择操作: ").strip()
        
        if choice == '1':  # 文本输入转换
            rules_file = input("使用规则文件 (回车使用默认): ").strip() or "default_rules.txt"
            text = input("输入要转换的文本:\n")
            
            if not text:
                print("输入不能为空!")
                continue
                
            success, result = converter.run_conversion(
                rules_file=converter.rules_dir / rules_file,
                input_text=text
            )
            
            if success:
                print("\n转换结果:")
                print("-" * 40)
                print(result)
                print("-" * 40)
            else:
                print(f"\n错误: {result}")
                # 添加更多调试信息
                print("可能的解决方案:")
                print("1. 检查规则文件是否存在且格式正确")
                print("2. 确保C++核心程序已正确编译")
                print("3. 尝试手动运行C++程序进行测试")
                print("4. 检查输出目录权限")
        
        elif choice == '2':  # 文件输入转换
            rules_file = input("使用规则文件 (回车使用默认): ").strip() or "default_rules.txt"
            input_file = input("输入文件路径 (在inputs目录下可直接输入文件名): ").strip()
            
            if not input_file:
                print("请输入文件路径")
                continue
                
            # 处理相对路径
            input_path = Path(input_file)
            if not input_path.is_absolute():
                input_path = converter.inputs_dir / input_file
            
            output_file = input("输出文件路径 (回车自动生成): ").strip()
            output_path = None
            if output_file:
                output_path = Path(output_file)
                if not output_path.is_absolute():
                    output_path = converter.outputs_dir / output_file
            
            success, result = converter.run_conversion(
                rules_file=converter.rules_dir / rules_file,
                input_text="",
                input_file=input_path,
                output_file=output_path
            )
            
            if success:
                print("\n转换结果:")
                print("-" * 40)
                print(result)
                print("-" * 40)
                print(f"结果已保存到: {output_path if output_path else 'outputs目录'}")
            else:
                print(f"错误: {result}")
        
        elif choice == '3':  # 规则文件管理
            print("\n规则文件管理:")
            print("a. 列出规则文件")
            print("b. 创建规则文件")
            print("c. 编辑规则文件")
            print("d. 查看规则文件内容")
            print("e. 返回主菜单")
            
            sub_choice = input("请选择: ").lower().strip()
            
            if sub_choice == 'a':
                files = converter.list_rule_files()
                print("\n可用规则文件:")
                for i, f in enumerate(files, 1):
                    print(f"{i}. {f}")
            
            elif sub_choice == 'b':
                filename = input("新规则文件名 (不含路径): ").strip()
                content = input("规则内容 (每行: 原词 替换词):\n")
                success, msg = converter.create_rule_file(filename, content)
                print(msg)
            
            elif sub_choice == 'c':
                filename = input("要编辑的规则文件名: ").strip()
                content = converter.get_rule_file_content(filename)
                if content is None:
                    print("文件不存在!")
                    continue
                    
                print("当前内容:")
                print("-" * 40)
                print(content)
                print("-" * 40)
                new_content = input("输入新内容 (直接回车取消):\n")
                
                if new_content.strip():
                    success, msg = converter.edit_rule_file(filename, new_content)
                    print(msg)
            
            elif sub_choice == 'd':
                filename = input("要查看的规则文件名: ").strip()
                content = converter.get_rule_file_content(filename)
                if content is None:
                    print("文件不存在!")
                else:
                    print("\n文件内容:")
                    print("-" * 40)
                    print(content)
                    print("-" * 40)
        
        elif choice == '4':  # 输入文件列表
            files = converter.list_input_files()
            if files:
                print("\n输入文件列表:")
                for i, f in enumerate(files, 1):
                    print(f"{i}. {f}")
            else:
                print("inputs目录为空")
        
        elif choice == '5':  # 输出文件列表
            files = converter.list_output_files()
            if files:
                print("\n输出文件列表:")
                for i, f in enumerate(files, 1):
                    print(f"{i}. {f}")
            else:
                print("outputs目录为空")
        
        elif choice == '6':  # 退出
            print("感谢使用!")
            break
        
        else:
            print("无效选择，请重试")

if __name__ == "__main__":
    main()