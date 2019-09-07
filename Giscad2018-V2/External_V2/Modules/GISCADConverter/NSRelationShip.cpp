#include "StdAfx.h"
#include "NSRelationShip.h"

// Constructor
CNSRelationShip::CNSRelationShip(void)
{
	m_pElement = NULL;
}

// Destructor
CNSRelationShip::~CNSRelationShip(void)
{
}

void CNSRelationShip::setElement( CNSTElement &element)
{
	m_pElement = &element;
}

/*!
@Brief			This function reads Connectiviy, Ownership and ManyOwner ship informaiton from XML.
@Return			If reading is successful then function returns NS_SUCCESS else function returns NS_FAIL.
@Date			12-06-2007
*/
int CNSRelationShip::readXML()
{
	CNSTElement oConnectivityNode = m_pElement->subnode(_T("Connectivity"));
	m_connectivity.setElement(oConnectivityNode);
	if(NS_SUCCESS != m_connectivity.readXML())
		return NS_FAIL;
	CNSTElement oOwnershipNode = m_pElement->subnode(_T("Ownership"));
	m_ownerShip.setElement(oOwnershipNode);
	if(NS_SUCCESS != m_ownerShip.readXML())
		return NS_FAIL;
	CNSTElement oManyOwnershipNode = m_pElement->subnode(_T("ManyOwnership"));
	m_manyOwnership.setElement(oManyOwnershipNode);
	if(NS_SUCCESS != m_manyOwnership.readXML())
		return NS_FAIL;
	return 0;
}
int CNSRelationShip::readDWG()
{
	return 0;
}

/*!
@Brief			This function writes the Relationship information i.e Connectivity, Ownership and many owenership
				information into output XML.
@Return			NS_SUCCESS	- If writing successfull
				NS_FAIL		- If writing not successful
@Date			12-06-2007
*/
int CNSRelationShip::writeXML()
{
	CNSTElement relationshipNode = m_pElement->createNode(_T("Relationships"), _T(""), DEFAULT_NAMESPACE);
	
	m_connectivity.setElement(relationshipNode);
	if(NS_SUCCESS != m_connectivity.writeXML())
		return NS_FAIL;

	m_ownerShip.setElement(relationshipNode);
	if(NS_SUCCESS != m_ownerShip.writeXML())
		return NS_FAIL;

	m_manyOwnership.setElement(relationshipNode);
	if(NS_SUCCESS != m_manyOwnership.writeXML())
		return NS_FAIL;
	

	return 0;
}
int CNSRelationShip::writeDWG()
{
	return 0;
}

/*!
@Brief			set information to String Map
@Param [in]		mapOfRelationships		:	
@Return			void
@Date			12-06-2007
*/
void CNSRelationShip::getXData(STRING_MAP& mapOfRelationships)
{
	m_connectivity.getXData(mapOfRelationships);
	m_ownerShip.getXData(mapOfRelationships);
	m_manyOwnership.getXData(mapOfRelationships);
}

/*!
@Brief			get information in String Map
@Param [in]		mapOfRelationships		:	
@Return			void
@Date			12-06-2007
*/
void CNSRelationShip::setXData(STRING_MAP& mapOfRelationships)
{
	m_connectivity.setXData(mapOfRelationships);
	m_ownerShip.setXData(mapOfRelationships);
	m_manyOwnership.setXData(mapOfRelationships);
}

bool CNSRelationShip::getConnectivityData(std::vector<int> &vectNode1, std::vector<int> &vectNode2)
{
	return m_connectivity.getConnectivityData(vectNode1, vectNode2);
}

