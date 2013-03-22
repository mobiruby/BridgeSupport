#!/usr/bin/macruby

require 'test/unit'

framework 'Cocoa'
framework 'QuartzCore'
framework 'ApplicationServices'
framework 'CoreFoundation'

class TestFoundation < Test::Unit::TestCase
    def test_performSelector
    	set = nil
    	assert_nothing_raised() {set = NSMutableSet.set}
	assert_kind_of(NSMutableSet, set)
	assert_nothing_raised() {set.addObject 'foo'}
	str = nil
	assert_nothing_raised() {str = set.performSelector 'description'}
	assert_kind_of(String, str)
    end
end

class TestQuartzCore < Test::Unit::TestCase
    def test_constants
	constant = nil
	assert_nothing_raised() {constant = CGPointZero}
	assert_kind_of(CGPoint, constant, 'CGPointZero')
	constant = nil
	assert_nothing_raised() {constant = CGRectInfinite}
	assert_kind_of(CGRect, constant, 'CGRectInfinite')
	constant = nil
	assert_nothing_raised() {constant = CGRectNull}
	assert_kind_of(CGRect, constant, 'CGRectNull')
	constant = nil
	assert_nothing_raised() {constant = CGSizeZero}
	assert_kind_of(CGSize, constant, 'CGSizeZero')
	constant = nil
	assert_nothing_raised() {constant = KCATransactionAnimationDuration}
	assert_kind_of(String, constant, 'KCATransactionAnimationDuration')
	constant = nil
	assert_nothing_raised() {constant = KCGMaxDisplayReservationInterval}
	assert_in_delta(15.0, constant, 0.0000001)
    end

    def test_NSAffineTransform
	t = nil
	assert_nothing_raised() {t = NSAffineTransform.transform}
	assert_kind_of(NSAffineTransform, t, 'NSAffineTransform.transform')
	assert_nothing_raised() {t.translateXBy 4, yBy: 1}
	assert_nothing_raised() {t.scaleXBy 0.5, yBy: 0.5}
	point = nil
	assert_nothing_raised() {point = CGPointMake(1, 1)}
	assert_kind_of(CGPoint, point, 'CGPointMake(1, 1)')
	transformed = nil
	assert_nothing_raised() {transformed = t.transformPoint point}
	assert_kind_of(CGPoint, transformed)
	result = nil
	assert_nothing_raised() {result = CGPointMake(4.5, 1.5)}
	assert_kind_of(CGPoint, point, 'CGPointMake(1, 1)')
	assert_equal(result, transformed)
	struct = nil
	assert_nothing_raised() {struct = t.transformStruct}
	assert_kind_of(NSAffineTransformStruct, struct)
	struct2 = nil
	assert_nothing_raised() {struct2 = NSAffineTransformStruct.new(0.5,0,0,0.5,4,1)}
	assert_kind_of(NSAffineTransformStruct, struct2)
	assert_equal(struct, struct2)
    end
end

class TestCoreGraphics < Test::Unit::TestCase
    def test_func_alias_inline
	t = nil
	assert_nothing_raised() {t = CGAffineTransformMake(1.0, 0, 0, 1.0, 0, 0)}
	assert_kind_of(CGAffineTransform, t, 'CGAffineTransformMake')
	assert_nothing_raised() {t = CGAffineTransformTranslate(t, 4, 1)}
	assert_nothing_raised() {t = CGAffineTransformScale(t, 0.5, 0.5)}
	point = nil
	assert_nothing_raised() {point = CGPointMake(1, 1)}
	assert_kind_of(CGPoint, point, 'CGPointMake(1, 1)')
	transformed = nil
	assert_nothing_raised() {transformed = CGPointApplyAffineTransform(point, t)}
	assert_kind_of(CGPoint, transformed)
	result = nil
	assert_nothing_raised() {result = CGPointMake(4.5, 1.5)}
	assert_kind_of(CGPoint, point, 'CGPointMake(1, 1)')
	assert(CGPointEqualToPoint(result, transformed), "result=#{result.inspect} transformed=#{transformed.inspect}")
    end
end

class TestCoreFoundation < Test::Unit::TestCase
    def test_CFMutableArray
    	array = nil
	id = nil
	arrayid = nil
    	assert_nothing_raised() {array = CFArrayCreateMutable(nil, 4, nil)}
    	assert_nothing_raised() {id = CFGetTypeID(array)}
    	assert_nothing_raised() {arrayid = CFArrayGetTypeID()}
	assert_equal(arrayid, id)
	assert_equal(0, CFArrayGetCount(array))
    	array << 'foo'
	assert_equal(1, CFArrayGetCount(array))
	assert_nothing_raised() do
	    str = Pointer.new_with_type('@')
	    str.assign("boo")
	    CFArrayAppendValue(array, str)
	end
	assert_equal(2, CFArrayGetCount(array))
	x = nil
	assert_nothing_raised() {x = CFArrayGetValueAtIndex(array, 0)}
	assert_kind_of(String, x, 'CFArrayGetValueAtIndex(array, 0)')
	assert_equal("foo", x)
	x = nil
	assert_nothing_raised() {x = CFArrayGetValueAtIndex(array, 1)}
	assert_kind_of(Pointer, x, 'CFArrayGetValueAtIndex(array, 1)')
	assert_kind_of(String, x[0], 'x[0] not a String')
	assert_equal("boo", x[0])
    end
end

class TestAddressBook < Test::Unit::TestCase
    @@pi = nil
    @@i = nil
    @@coun = nil
    @@people = nil
    @@CFAPIsOK = false
    @@primaryIdentifier = nil
    @@primaryIndex = nil
    @@country = nil
    @@peopleFound = nil
    @@ObjCAPIsOK = false

    def compareAPIs
	#puts 'In compareAPIs' #DEBUG
	assert_equal(@@pi, @@primaryIdentifier)
	assert_equal(@@i, @@primaryIndex)
	assert_equal(@@coun, @@country)
	assert_equal(@@people, @@peopleFound)
    end

    @@AddressBookLoadAttempted = false
    @@AddressBookLoaded = false
    def loadAddressBook
	return if @@AddressBookLoadAttempted
	@@AddressBookLoadAttempted = true
	assert_nothing_raised do
	    # the plain framework name sometimes fails
	    #framework 'AddressBook'
	    framework '/System/Library/Frameworks/AddressBook.framework'
	    @@AddressBookLoaded = true
	end
    end

    def test_CoreFoundationAPIs
	loadAddressBook
	return unless @@AddressBookLoaded
	#puts 'In test_CoreFoundationAPIs' #DEBUG
	ab = nil
	assert_nothing_raised() {ab = ABGetSharedAddressBook()}
	assert_kind_of(ABAddressBook, ab, 'ABGetSharedAddressBook()')
	me = nil
	assert_nothing_raised() {me = ABGetMe(ab)}
	assert_kind_of(ABPerson, me, 'ABGetMe()')
	alist = nil
	assert_nothing_raised() {alist = ABRecordCopyValue(me, KABAddressProperty)}
	assert_kind_of(ABMultiValueCoreDataWrapper, alist, 'ABRecordCopyValue(me, KABAddressProperty)')
	assert_nothing_raised() {@@pi = ABMultiValueCopyPrimaryIdentifier(alist)}
	assert_kind_of(String, @@pi, 'ABMultiValueCopyPrimaryIdentifier(alist)')
	assert_nothing_raised() {@@i = ABMultiValueIndexForIdentifier(alist, @@pi)}
	assert_kind_of(Integer, @@i, 'ABMultiValueIndexForIdentifier(alist, @@pi)')
	addr = nil
	assert_nothing_raised() {addr = ABMultiValueCopyValueAtIndex(alist, @@i)}
	assert_kind_of(Hash, addr, 'ABMultiValueCopyValueAtIndex(alist, @@i)')
	assert_nothing_raised() {@@coun = addr[KABAddressCountryKey]}
	assert_kind_of(String, @@coun, 'addr[KABAddressCountryKey]')
	same = nil
	assert_nothing_raised() {same = ABPersonCreateSearchElement(KABAddressProperty, nil, KABAddressCountryKey, @@coun, KABEqualCaseInsensitive)}
	assert_kind_of(ABSearchElementMatch, same, 'ABPersonCreateSearchElement(KABAddressProperty, nil, KABAddressCountryKey, @@coun, KABEqualCaseInsensitive)')
	assert_nothing_raised() {@@people = ABCopyArrayOfMatchingRecords(ab, same)}
	assert_kind_of(Array, @@people, 'ABCopyArrayOfMatchingRecords(ab, same)')
	@@CFAPIsOK = true
	compareAPIs if @@ObjCAPIsOK
    end

    def test_ObjCAPIs
	loadAddressBook
	return unless @@AddressBookLoaded
	#puts 'In test_ObjCAPIs' #DEBUG
	aB = nil
	assert_nothing_raised() {aB = ABAddressBook.sharedAddressBook}
	assert_kind_of(ABAddressBook, aB, 'ABAddressBook.sharedAddressBook')
	aPerson = nil
	assert_nothing_raised() {aPerson = aB.me}
	assert_kind_of(ABPerson, aPerson, 'aB.me')
	anAddressList = nil
	assert_nothing_raised() {anAddressList = aPerson.valueForProperty(KABAddressProperty)}
	assert_kind_of(ABMultiValueCoreDataWrapper, anAddressList, 'aPerson.valueForProperty(KABAddressProperty)')
	assert_nothing_raised() {@@primaryIdentifier = anAddressList.primaryIdentifier}
	assert_kind_of(String, @@primaryIdentifier, 'anAddressList.primaryIdentifier')
	assert_nothing_raised() {@@primaryIndex = anAddressList.indexForIdentifier(@@primaryIdentifier)}
	assert_kind_of(Integer, @@primaryIndex, 'anAddressList.indexForIdentifier(primaryIdentifier)')
	anAddress = nil
	assert_nothing_raised() {anAddress = anAddressList.valueAtIndex(@@primaryIndex)}
	assert_kind_of(Hash, anAddress, 'anAddressList.valueAtIndex(@@primaryIndex)')
	assert_nothing_raised() {@@country = anAddress.objectForKey(KABAddressCountryKey)}
	assert_kind_of(String, @@country, 'anAddress.objectForKey(KABAddressCountryKey)')
	sameCountry = nil
	assert_nothing_raised() {sameCountry = ABPerson.searchElementForProperty(KABAddressProperty, label:nil, key:KABAddressCountryKey, value:@@country, comparison:KABEqualCaseInsensitive)}
	assert_kind_of(ABSearchElementMatch, sameCountry, 'ABPerson.searchElementForProperty(KABAddressProperty, label:nil, key:KABAddressCountryKey, value:@@country, comparison:KABEqualCaseInsensitive)')
	assert_nothing_raised() {@@peopleFound = aB.recordsMatchingSearchElement(sameCountry)}
	assert_kind_of(Array, @@peopleFound, 'aB.recordsMatchingSearchElement(sameCountry)')
	@@ObjCAPIsOK = true
	compareAPIs if @@CFAPIsOK
    end
end
