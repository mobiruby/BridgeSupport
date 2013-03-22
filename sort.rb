# A naive tool to sort the content of exception files.

require 'rexml/document'

unless ARGV.size == 1
  $stderr.puts "Usage: #{__FILE__} <exception>"
  exit 1
end

doc = REXML::Document.new(File.read(ARGV.first))
root = doc.root

def do_sorting(element, xpath, attr_name)
  elements = element.get_elements(xpath).to_a
  sentinel = REXML::Element.new
  element.insert_before(elements.first, sentinel)
  elements.sort { |x, y| 
    x.attributes[attr_name] <=> y.attributes[attr_name]
  }.reverse.each { |new_e|
    element.delete_element(new_e)
    element.insert_after(sentinel, new_e)
  }
  element.delete_element(sentinel)
end

%w{struct cftype opaque function class}.each { |n| do_sorting(root, n, 'name') }
root.each_element('class') { |elem| do_sorting(elem, 'method', 'selector') }

doc.write($stdout)
