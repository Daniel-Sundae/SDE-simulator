import argparse
import os
import re
import shutil

def collect_method_names(dir_path):
    method_dict = {}
    sig_pattern = re.compile(
        r'(?:(inline|template\s*<.*?>)\s+)?auto\s+(?:[\w:]+::)?([A-Z][a-zA-Z0-9_]*)\s*\(',
        re.MULTILINE | re.DOTALL
    )
    for root, _, files in os.walk(dir_path):
        for file in files:
            if file.endswith(('.cpp', '.h', '.hpp', '.inl')):
                file_path = os.path.join(root, file)
                try:
                    with open(file_path, 'r', encoding='utf-8') as f:
                        content = f.read()
                    for match in sig_pattern.finditer(content):
                        method_name = match.group(2)
                        camel_method = method_name[0].lower() + method_name[1:]
                        method_dict[method_name] = camel_method
                except Exception as e:
                    print(f"Error scanning {file_path}: {e}")
    return method_dict

def refactor_file(file_path, method_dict, make_backup=False):
    if make_backup:
        try:
            shutil.copy(file_path, f"{file_path}.bak")
        except Exception as e:
            print(f"Error creating backup for {file_path}: {e}")
            return
    
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            original_content = f.read()
    except Exception as e:
        print(f"Error reading {file_path}: {e}")
        return
    
    content = original_content
    
    # Common qualifiers pattern
    qualifiers_pat = r'((?:\s*(?:noexcept|const|volatile|&|&&|override|final|=[ \t]*0))*)'
    
    # Optional template/inline prefix
    prefix_pat = r'(?:(inline|template\s*<.*?>)\s+)?'
    
    # Pattern 1: Class out-of-line with trailing return
    out_trailing = re.compile(
        prefix_pat + r'auto\s+([\w:]+)::([A-Z][a-zA-Z0-9_]*)\s*\((.*?)\)' + qualifiers_pat + r'\s*->\s*([^;{]+?)\s*([;{])',
        re.MULTILINE | re.DOTALL | re.VERBOSE
    )
    
    def out_trailing_replacer(match):
        prefix, class_name, method_name, params, qualifiers, return_type, terminator = match.groups()
        if '[]' in match.group(0) or 'auto lambda' in match.group(0):
            return match.group(0)
        camel_method = method_name[0].lower() + method_name[1:]
        prefix = prefix or ''
        return f"{prefix}{return_type.strip()} {class_name}::{camel_method}({params.strip()}){qualifiers}{terminator}"
    
    content = out_trailing.sub(out_trailing_replacer, content)
    
    # Pattern 2: Class in-class with trailing return
    in_trailing = re.compile(
        prefix_pat + r'(\[\[.*?\]\]\s*)?auto\s+([A-Z][a-zA-Z0-9_]*)\s*\((.*?)\)' + qualifiers_pat + r'\s*->\s*([^;{]+?)\s*([;{])',
        re.MULTILINE | re.DOTALL | re.VERBOSE
    )
    
    def in_trailing_replacer(match):
        prefix, attributes, method_name, params, qualifiers, return_type, terminator = match.groups()
        if '[]' in match.group(0) or 'auto lambda' in match.group(0):
            return match.group(0)
        camel_method = method_name[0].lower() + method_name[1:]
        prefix = prefix or ''
        attributes = attributes or ''
        return f"{prefix}{attributes}{return_type.strip()} {camel_method}({params.strip()}){qualifiers}{terminator}"
    
    content = in_trailing.sub(in_trailing_replacer, content)
    
    # Pattern 3: Class out-of-line without trailing return (keep auto)
    out_no_trailing = re.compile(
        prefix_pat + r'auto\s+([\w:]+)::([A-Z][a-zA-Z0-9_]*)\s*\((.*?)\)' + qualifiers_pat + r'\s*([;{])',
        re.MULTILINE | re.DOTALL | re.VERBOSE
    )
    
    def out_no_trailing_replacer(match):
        prefix, class_name, method_name, params, qualifiers, terminator = match.groups()
        if '[]' in match.group(0) or 'auto lambda' in match.group(0):
            return match.group(0)
        camel_method = method_name[0].lower() + method_name[1:]
        prefix = prefix or ''
        return f"{prefix}auto {class_name}::{camel_method}({params.strip()}){qualifiers}{terminator}"
    
    content = out_no_trailing.sub(out_no_trailing_replacer, content)
    
    # Pattern 4: Class in-class without trailing return (keep auto)
    in_no_trailing = re.compile(
        prefix_pat + r'(\[\[.*?\]\]\s*)?auto\s+([A-Z][a-zA-Z0-9_]*)\s*\((.*?)\)' + qualifiers_pat + r'\s*([;{])',
        re.MULTILINE | re.DOTALL | re.VERBOSE
    )
    
    def in_no_trailing_replacer(match):
        prefix, attributes, method_name, params, qualifiers, terminator = match.groups()
        if '[]' in match.group(0) or 'auto lambda' in match.group(0):
            return match.group(0)
        camel_method = method_name[0].lower() + method_name[1:]
        prefix = prefix or ''
        attributes = attributes or ''
        return f"{prefix}{attributes}auto {camel_method}({params.strip()}){qualifiers}{terminator}"
    
    content = in_no_trailing.sub(in_no_trailing_replacer, content)
    
    # Pattern 5: Free function with trailing return
    free_trailing = re.compile(
        prefix_pat + r'auto\s+([A-Z][a-zA-Z0-9_]*)\s*\((.*?)\)' + qualifiers_pat + r'\s*->\s*([^;{]+?)\s*([;{])',
        re.MULTILINE | re.DOTALL | re.VERBOSE
    )
    
    def free_trailing_replacer(match):
        prefix, method_name, params, qualifiers, return_type, terminator = match.groups()
        if '[]' in match.group(0) or 'auto lambda' in match.group(0):
            return match.group(0)
        camel_method = method_name[0].lower() + method_name[1:]
        prefix = prefix or ''
        return f"{prefix}{return_type.strip()} {camel_method}({params.strip()}){qualifiers}{terminator}"
    
    content = free_trailing.sub(free_trailing_replacer, content)
    
    # Pattern 6: Free function without trailing return (keep auto)
    free_no_trailing = re.compile(
        prefix_pat + r'auto\s+([A-Z][a-zA-Z0-9_]*)\s*\((.*?)\)' + qualifiers_pat + r'\s*([;{])',
        re.MULTILINE | re.DOTALL | re.VERBOSE
    )
    
    def free_no_trailing_replacer(match):
        prefix, method_name, params, qualifiers, terminator = match.groups()
        if '[]' in match.group(0) or 'auto lambda' in match.group(0):
            return match.group(0)
        camel_method = method_name[0].lower() + method_name[1:]
        prefix = prefix or ''
        return f"{prefix}auto {camel_method}({params.strip()}){qualifiers}{terminator}"
    
    content = free_no_trailing.sub(free_no_trailing_replacer, content)
    
    # Pattern 7: Call sites (handle optional qualifiers like Namespace::OldName < or ( )
    for old_name, new_name in method_dict.items():
        call_pattern = re.compile(
            r'((?:[\w:]+::)?)' + re.escape(old_name) + r'\s*([<(])',
            re.MULTILINE
        )
        content = call_pattern.sub(lambda m: m.group(1) + new_name + m.group(2), content)
    
    if content != original_content:
        try:
            with open(file_path, 'w', encoding='utf-8') as f:
                f.write(content)
            print(f"Refactored: {file_path}")
        except Exception as e:
            print(f"Error writing to {file_path}: {e}")

def main():
    parser = argparse.ArgumentParser(description="Refactor C++ methods.")
    parser.add_argument("dir", help="Project directory")
    parser.add_argument("--backup", action="store_true", help="Create backups")
    args = parser.parse_args()
    
    # Pass 1: Collect all refactored method names
    method_dict = collect_method_names(args.dir)
    
    # Pass 2: Refactor files using the dict
    for root, _, files in os.walk(args.dir):
        for file in files:
            if file.endswith(('.cpp', '.h', '.hpp', '.inl')):
                refactor_file(os.path.join(root, file), method_dict, args.backup)

if __name__ == "__main__":
    main()